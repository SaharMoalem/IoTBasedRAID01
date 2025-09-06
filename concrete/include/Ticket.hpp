#ifndef  ILRD_RD1645_TICKET_HPP
#define  ILRD_RD1645_TICKET_HPP

#include "AMessage.hpp"
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

class Ticket
{
public:
    explicit Ticket(const UID& uid);
    ~Ticket() = default;
    void RegisterToResultDispatcher(ACallback<std::shared_ptr<TaskResult>>& callback);
    void CheckTicketDone(const std::shared_ptr<AMessage>& message);

private:
    UID m_uid;
    size_t m_done;
    Dispatcher<std::shared_ptr<TaskResult>> m_dispatcher;
};
}

#endif