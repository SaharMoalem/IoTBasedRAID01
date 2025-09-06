#include <algorithm>

#include "MinionManager.hpp"
#include "Ticket.hpp"
#include "factory.hpp"
#include "MessageReceive.hpp"
#include "handleton.hpp"
#include "logger.hpp"

using namespace ilrd;

MinionManager::~MinionManager()
{
    m_reactor.Stop();
}

void MinionManager::Init(size_t mbMinion,
                    const std::vector<std::shared_ptr<IMinionProxy>>& minions)
{
    m_MBMinion = mbMinion;
    std::copy(minions.begin(), minions.end(), std::back_inserter(m_minions));
    
    for(auto minion : m_minions)
    {
        m_reactor.Register(minion->GetMinionFD(), IListener::READ,
            [minion](int fd, IListener::Mode mode) {
                (void)fd;
                (void)mode;
                minion->OnMinionFDWakeUp();
            });
    }

    Handleton::GetInstance<Factory<uint32_t,
        AMessage>>()->Register(MESSAGE_TYPE::READ_RECEIVE, []() {
           return std::make_shared<MessageReadReceive>(); 
        });

    Handleton::GetInstance<Factory<uint32_t,
        AMessage>>()->Register(MESSAGE_TYPE::WRITE_RECEIVE, []() {
           return std::make_shared<MessageWriteReceive>(); 
        });

    m_worker = std::jthread([this]() {
        m_reactor.Run();
        }); 
}

void MinionManager::AddReadTask(const UID& uid, size_t offset, size_t length)
{
    std::shared_ptr<Ticket> ticket = std::make_shared<Ticket>(uid);
    m_dispatcher.Notify(std::make_pair(ticket, uid));
    
    m_minions[offset / m_MBMinion]->AddReadTask(uid, offset % m_MBMinion,
        length, [ticket](const std::shared_ptr<AMessage>& msg) {
            ticket->CheckTicketDone(msg);
        });

    m_minions[(offset / m_MBMinion + 1) % m_minions.size()]->AddReadTask(uid,
        offset % m_MBMinion + m_MBMinion, length,
        [ticket](const std::shared_ptr<AMessage>& msg) {
            ticket->CheckTicketDone(msg);
        }); // backup
}

void MinionManager::AddWriteTask(const UID& uid, size_t offset, size_t length,
                                        const std::shared_ptr<char[]>& buffer)
{
    std::shared_ptr<Ticket> ticket(std::make_shared<Ticket>(uid));
    m_dispatcher.Notify(std::make_pair(ticket, uid));

    m_minions[offset / m_MBMinion]->AddWriteTask(uid, offset % m_MBMinion,
        length, buffer, [ticket](const std::shared_ptr<AMessage>& msg) {
            ticket->CheckTicketDone(msg);
        });

    m_minions[(offset / m_MBMinion + 1) % m_minions.size()]->AddWriteTask(uid,
        offset % m_MBMinion + m_MBMinion, length, buffer,
        [ticket](const std::shared_ptr<AMessage>& msg) {
            ticket->CheckTicketDone(msg);
        }); //backup
}

void MinionManager::RegisterForNewTickets(ACallback<std::pair<std::shared_ptr<Ticket>,
                                                                UID>>& callback)
{
    m_dispatcher.Register(&callback);
}