#include <iostream>

#include "MinionProxy.hpp"
#include "MessageSend.hpp"
#include "MessageReceive.hpp"
#include "handleton.hpp"
#include "factory.hpp"
#include "logger.hpp"

using namespace ilrd;

MinionProxy::MinionProxy(const std::string& ip, const std::string& port):
    m_socket(port, ip.c_str()) {}

void MinionProxy::AddReadTask(const UID& uid, size_t offset, size_t length,
                std::function<void(const std::shared_ptr<AMessage>&)> onDone)
{
    MessageReadSend sMsg(uid.GetID(), offset, length);
    char* buffer = new char[sMsg.GetSize()];
    sMsg.ToBuffer(buffer);

    {
        std::unique_lock lock(m_mtx);
        m_map[uid] = onDone;
        m_socket.SendTo(buffer, sMsg.GetSize());
    }

    Handleton::GetInstance<Logger>()->Log("Master sent: " +
        std::to_string(sMsg.GetUID()) + " " + std::to_string(sMsg.GetOffset()) +
                        " " + std::to_string(sMsg.GetLength()), Logger::DEBUG);
    delete[] buffer;
}

void MinionProxy::AddWriteTask(const UID& uid, size_t offset, size_t length,
                                            const std::shared_ptr<char[]>& data,
                std::function<void(const std::shared_ptr<AMessage>&)> onDone)
{
    MessageWriteSend sMsg(uid.GetID(), offset, length, data);
    char* buffer = new char[sMsg.GetSize()];
    sMsg.ToBuffer(buffer);

    {
        std::unique_lock lock(m_mtx);
        m_map[uid] = onDone;
        m_socket.SendTo(buffer, sMsg.GetSize());
    }
    
    delete[] buffer;
}

int MinionProxy::GetMinionFD()
{
    return m_socket.GetFD();
}

void MinionProxy::OnMinionFDWakeUp()
{
    char buffer[BUFSIZ];

    {
        std::unique_lock lock(m_mtx);
        m_socket.ReceiveFrom(buffer, BUFSIZ - 1);
    }
    
    char* runner = buffer;
    runner += sizeof(uint32_t);
    uint32_t messageType = *(uint32_t*)runner;
    runner += sizeof(uint32_t);
    std::shared_ptr<AMessage> receivedMessage = Handleton::GetInstance<Factory<uint32_t, AMessage>>()->Create(messageType);
    receivedMessage->FromBuffer(runner);
    UID uid(receivedMessage->GetUID());

    std::unique_lock lock(m_mtx);
    m_map[uid](receivedMessage);
    m_map.erase(uid);
}