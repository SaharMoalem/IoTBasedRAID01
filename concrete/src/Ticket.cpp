#include "Ticket.hpp"

using namespace ilrd;

Ticket::Ticket(const UID& uid): m_uid(uid), m_done(0){}

void Ticket::CheckTicketDone(const std::shared_ptr<AMessage>& message)
{
    if(++m_done == 1)
    {
        TaskResult result = {UID(message->GetUID()), message->GetResult(), message->GetLength(), message->GetData()};
        m_dispatcher.Notify(std::make_shared<TaskResult>(result));
    }
}

void Ticket::RegisterToResultDispatcher(ACallback<std::shared_ptr<TaskResult>>& callback)
{
    m_dispatcher.Register(&callback);
}