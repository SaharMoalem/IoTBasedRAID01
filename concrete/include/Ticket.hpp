#ifndef  ILRD_RD1645_TICKET_HPP
#define  ILRD_RD1645_TICKET_HPP

#include <mutex>
#include <vector>

#include "AMessage.hpp"
#include "StripeSplitter.hpp"
#include "dispatcher.hpp"
#include "IMinionProxy.hpp"

namespace ilrd
{

struct TaskResult
{
    UID uid;
    bool result;
    size_t length;
    std::shared_ptr<char[]> buffer;
};

enum class TicketOp
{
    READ,
    WRITE
};

enum class MirrorRole
{
    PRIMARY,
    BACKUP
};

class Ticket
{
public:
    Ticket(const UID& uid, TicketOp op, size_t totalLength,
           const std::vector<StripeSegment>& segments);
    ~Ticket() = default;

    void RegisterToResultDispatcher(ACallback<std::shared_ptr<TaskResult>>& callback);
    void OnMirrorResponse(size_t segmentIdx, MirrorRole role,
                          const std::shared_ptr<AMessage>& message);

private:
    struct SegmentState
    {
        size_t length;
        size_t bufferOffset;
        bool primaryDone = false;
        bool backupDone = false;
        std::shared_ptr<AMessage> primaryMsg;
        std::shared_ptr<AMessage> backupMsg;
    };

    bool IsSegmentResolved(const SegmentState& seg) const;
    bool IsSegmentSuccessful(const SegmentState& seg) const;
    std::shared_ptr<AMessage> SelectSegmentMessage(const SegmentState& seg) const;
    void TryComplete();

    UID m_uid;
    TicketOp m_op;
    size_t m_totalLength;
    std::vector<SegmentState> m_segments;
    bool m_completed;
    std::mutex m_mtx;
    Dispatcher<std::shared_ptr<TaskResult>> m_dispatcher;
};

}

#endif
