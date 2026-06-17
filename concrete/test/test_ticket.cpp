#include <iostream>
#include <memory>

#include "Ticket.hpp"
#include "MessageReceive.hpp"
#include "dispatcher.hpp"

#define TEST_ASSERT(cond)                                                      \
    do                                                                         \
    {                                                                          \
        if(!(cond))                                                            \
        {                                                                      \
            std::cerr << "FAIL: " << #cond << " at " << __FILE__ << ":"        \
                      << __LINE__ << "\n";                                     \
            return 1;                                                          \
        }                                                                      \
    } while(0)

using namespace ilrd;

class TicketCallback : public ACallback<std::shared_ptr<TaskResult>>
{
public:
    std::shared_ptr<TaskResult> result;

    void Notify(std::shared_ptr<TaskResult> event) override
    {
        result = event;
    }
};

int main()
{
    const UID uid(42);
    std::vector<StripeSegment> segments = {{0, 4, 0, 0, 1}};

    {
        Ticket ticket(uid, TicketOp::READ, 4, segments);
        TicketCallback cb;
        ticket.RegisterToResultDispatcher(cb);

        auto primaryFail = std::make_shared<MessageReadReceive>(42, false, 4, nullptr);
        auto backupOk = std::make_shared<MessageReadReceive>(42, true, 4,
            std::shared_ptr<char[]>(new char[4]{'a', 'b', 'c', 'd'}));

        ticket.OnMirrorResponse(0, MirrorRole::PRIMARY, primaryFail);
        TEST_ASSERT(!cb.result);
        ticket.OnMirrorResponse(0, MirrorRole::BACKUP, backupOk);
        TEST_ASSERT(cb.result);
        TEST_ASSERT(cb.result->result);
        TEST_ASSERT(cb.result->length == 4);
        TEST_ASSERT(cb.result->buffer);
        TEST_ASSERT(cb.result->buffer[0] == 'a');
    }

    {
        Ticket ticket(uid, TicketOp::WRITE, 4, segments);
        TicketCallback cb;
        ticket.RegisterToResultDispatcher(cb);

        auto primaryOk = std::make_shared<MessageWriteReceive>(42, true);
        auto backupFail = std::make_shared<MessageWriteReceive>(42, false);

        ticket.OnMirrorResponse(0, MirrorRole::PRIMARY, primaryOk);
        TEST_ASSERT(!cb.result);
        ticket.OnMirrorResponse(0, MirrorRole::BACKUP, backupFail);
        TEST_ASSERT(cb.result);
        TEST_ASSERT(!cb.result->result);
    }

    {
        Ticket ticket(uid, TicketOp::WRITE, 4, segments);
        TicketCallback cb;
        ticket.RegisterToResultDispatcher(cb);

        auto primaryOk = std::make_shared<MessageWriteReceive>(42, true);
        auto backupOk = std::make_shared<MessageWriteReceive>(42, true);

        ticket.OnMirrorResponse(0, MirrorRole::PRIMARY, primaryOk);
        ticket.OnMirrorResponse(0, MirrorRole::BACKUP, backupOk);
        TEST_ASSERT(cb.result);
        TEST_ASSERT(cb.result->result);
    }

    std::cout << "ticket mirror ok\n";
    return 0;
}
