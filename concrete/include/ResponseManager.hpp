#ifndef  ILRD_RD1645_RESPONSEMANAGER_HPP
#define  ILRD_RD1645_RESPONSEMANAGER_HPP

#include <map>

#include "handleton.hpp"
#include "ATaskArgs.hpp"
#include "IResponse.hpp"
#include "NBDProxy.hpp"
#include "Ticket.hpp"
#include "MinionManager.hpp"

namespace ilrd
{
class ResponseManager
{
public:
    void Init(NBDProxy& proxy);

private:
    friend class Handleton;
    ResponseManager();
    void OnNewTask(std::shared_ptr<ATaskArgs> args);
    void OnTicketCreate(std::pair<std::shared_ptr<Ticket>, UID> pair);
    void OnTicketDone(std::shared_ptr<TaskResult> result);

    std::map<UID, std::shared_ptr<IResponse>> m_requests;
    std::map<UID, std::shared_ptr<Ticket>> m_tickets;
    Callback<std::shared_ptr<ATaskArgs>, ResponseManager> m_proxyCallback;
    Callback<std::shared_ptr<TaskResult>, ResponseManager> m_ticketDoneCallback;
    Callback<std::pair<std::shared_ptr<Ticket>, UID>, ResponseManager> m_newTicketCallback;
    NBDProxy* m_proxy;
    std::mutex m_mtx;
};
}

#endif