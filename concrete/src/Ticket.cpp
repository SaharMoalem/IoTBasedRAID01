#include <algorithm>
#include <cstring>

#include "Ticket.hpp"
#include "handleton.hpp"
#include "logger.hpp"

using namespace ilrd;

Ticket::Ticket(const UID& uid, TicketOp op, size_t totalLength,
               const std::vector<StripeSegment>& segments):
    m_uid(uid), m_op(op), m_totalLength(totalLength), m_completed(false)
{
    m_segments.reserve(segments.size());

    for(const StripeSegment& seg : segments)
    {
        m_segments.push_back({seg.length, seg.bufferOffset, false, false, nullptr, nullptr});
    }
}

void Ticket::RegisterToResultDispatcher(ACallback<std::shared_ptr<TaskResult>>& callback)
{
    m_dispatcher.Register(&callback);
}

void Ticket::OnMirrorResponse(size_t segmentIdx, MirrorRole role,
                              const std::shared_ptr<AMessage>& message)
{
    std::unique_lock lock(m_mtx);

    if(m_completed || segmentIdx >= m_segments.size())
    {
        return;
    }

    SegmentState& seg = m_segments[segmentIdx];

    if(role == MirrorRole::PRIMARY)
    {
        seg.primaryDone = true;
        seg.primaryMsg = message;
    }
    else
    {
        seg.backupDone = true;
        seg.backupMsg = message;
    }

    TryComplete();
}

bool Ticket::IsSegmentResolved(const SegmentState& seg) const
{
    if(m_op == TicketOp::WRITE)
    {
        return seg.primaryDone && seg.backupDone;
    }

    if(seg.primaryDone && seg.primaryMsg && seg.primaryMsg->GetResult())
    {
        return true;
    }

    return seg.backupDone;
}

bool Ticket::IsSegmentSuccessful(const SegmentState& seg) const
{
    if(m_op == TicketOp::WRITE)
    {
        return seg.primaryMsg && seg.primaryMsg->GetResult() &&
               seg.backupMsg && seg.backupMsg->GetResult();
    }

    const std::shared_ptr<AMessage> chosen = SelectSegmentMessage(seg);
    return chosen && chosen->GetResult();
}

std::shared_ptr<AMessage> Ticket::SelectSegmentMessage(const SegmentState& seg) const
{
    if(seg.primaryDone && seg.primaryMsg && seg.primaryMsg->GetResult())
    {
        return seg.primaryMsg;
    }

    if(seg.backupDone)
    {
        return seg.backupMsg;
    }

    return seg.primaryMsg;
}

void Ticket::TryComplete()
{
    for(const SegmentState& seg : m_segments)
    {
        if(!IsSegmentResolved(seg))
        {
            return;
        }
    }

    const bool success = std::all_of(m_segments.begin(), m_segments.end(),
        [this](const SegmentState& seg) { return IsSegmentSuccessful(seg); });

    TaskResult result;
    result.uid = m_uid;
    result.result = success;
    result.length = m_totalLength;
    result.buffer = nullptr;

    if(success && m_op == TicketOp::READ)
    {
        result.buffer.reset(new char[m_totalLength]);

        for(const SegmentState& seg : m_segments)
        {
            const std::shared_ptr<AMessage> msg = SelectSegmentMessage(seg);
            if(msg && msg->GetData())
            {
                std::memcpy(result.buffer.get() + seg.bufferOffset,
                    msg->GetData().get(), seg.length);
            }
        }
    }

    m_completed = true;
    m_dispatcher.Notify(std::make_shared<TaskResult>(result));
}
