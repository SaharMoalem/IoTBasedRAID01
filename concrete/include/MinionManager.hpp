#ifndef  ILRD_RD1645_MINIONMANAGER_HPP
#define  ILRD_RD1645_MINIONMANAGER_HPP

#include <thread>

#include "IMinionProxy.hpp"
#include "reactor.hpp"
#include "dispatcher.hpp"
#include "Ticket.hpp"
#include "handleton.hpp"
#include "ATaskArgs.hpp"

namespace ilrd
{

class MinionManager
{
public:
    ~MinionManager();
    void Init(size_t mbMinion, const std::vector<std::shared_ptr<IMinionProxy>>& minions);
    void AddReadTask(const UID& uid, size_t offset, size_t length);
    void AddWriteTask(const UID& uid, size_t offset, size_t length, const std::shared_ptr<char[]>& buffer);
    void RegisterForNewTickets(ACallback<std::pair<std::shared_ptr<Ticket>, UID>>& callback);

private:
    friend class Handleton;
    MinionManager() = default;

    size_t m_MBMinion;
    std::vector<std::shared_ptr<IMinionProxy>> m_minions;
    Dispatcher<std::pair<std::shared_ptr<Ticket>, UID>> m_dispatcher;
    Reactor m_reactor;
    std::jthread m_worker;
};
}

#endif