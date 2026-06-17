#include "ResponseManager.hpp"
#include "Response.hpp"
#include "handleton.hpp"
#include "logger.hpp"

using namespace ilrd;

ResponseManager::ResponseManager():
    m_proxyCallback((&ResponseManager::OnNewTask), *this),
    m_ticketDoneCallback((&ResponseManager::OnTicketDone), *this),
    m_newTicketCallback((&ResponseManager::OnTicketCreate), *this)
{
    //register to response factory?
}

void ResponseManager::Init(NBDProxy& proxy)
{
    m_proxy = &proxy;//?
    // proxy.RegisterForNewTaskArgs(m_proxyCallback);
    m_proxy->RegisterForNewTaskArgs(m_proxyCallback);
    Handleton::GetInstance<MinionManager>()->RegisterForNewTickets(m_newTicketCallback);
}

void ResponseManager::OnNewTask(std::shared_ptr<ATaskArgs> args)
{
    std::unique_lock lock(m_mtx);
    m_requests.emplace(args->GetUID(), std::make_shared<Response>());
}

void ResponseManager::OnTicketCreate(std::pair<std::shared_ptr<Ticket>, UID> pair)
{
    {
        std::unique_lock lock(m_mtx);
        m_tickets[pair.second] = pair.first;
    }
    
    pair.first->RegisterToResultDispatcher(m_ticketDoneCallback);
}

void ResponseManager::OnTicketDone(std::shared_ptr<TaskResult> result)
{
    {
        std::unique_lock lock(m_mtx);
        auto req_it = m_requests.find(result->uid);
        if(req_it == m_requests.end())
        {
            Handleton::GetInstance<Logger>()->Log(
                "ResponseManager: unknown UID in OnTicketDone", Logger::WARNING);
            return;
        }

        if(result->result)
        {
            req_it->second->OnSuccess();
        }
        else
        {
            req_it->second->OnFailure();
        }

        m_tickets.erase(result->uid);
        m_requests.erase(req_it);
        m_proxy->OnNBDResponse(result);
    }
}