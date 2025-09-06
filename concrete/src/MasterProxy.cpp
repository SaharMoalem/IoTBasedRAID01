#include <iostream>

#include "MasterProxy.hpp"
#include "factory.hpp"
#include "MinionArgs.hpp"
#include "logger.hpp"


using namespace ilrd;

void MasterProxy::Init(const std::string& port)
{
    m_socket.emplace(port);
    Handleton::GetInstance<Factory<uint32_t, ATaskArgs, size_t, TaskArgs>>()->Register(MESSAGE_TYPE::READ_SEND,
        [](size_t uid, const TaskArgs& args) {
        return std::make_shared<MinionReadArgs>(uid, args);
    });

    Handleton::GetInstance<Factory<uint32_t, ATaskArgs, size_t, TaskArgs>>()->Register(MESSAGE_TYPE::WRITE_SEND,
        [](size_t uid, const TaskArgs& args) {
        return std::make_shared<MinionWriteArgs>(uid, args);
    });

    Handleton::GetInstance<Factory<uint32_t, AMessage>>()->Register(MESSAGE_TYPE::READ_SEND,
        []() {
           return std::make_shared<MessageReadSend>(); 
    });

    Handleton::GetInstance<Factory<uint32_t, AMessage>>()->Register(MESSAGE_TYPE::WRITE_SEND,
        []() {
           return std::make_shared<MessageWriteSend>(); 
    });
}

std::shared_ptr<ITaskArgs> MasterProxy::GetTaskArgs(int fd, FDMODE mode)
{
    (void)fd;
    (void)mode;
    char buffer[BUFSIZ];

    {
        std::unique_lock lock(m_mtx);
        m_socket.value().ReceiveFrom(buffer, BUFSIZ - 1);
    }

    char* runner = buffer;
    runner += sizeof(uint32_t); 
    uint32_t messageType = *(uint32_t*)runner;
    runner += sizeof(uint32_t);
    std::shared_ptr<AMessage> inputMessage = Handleton::GetInstance<Factory<uint32_t, AMessage>>()->Create(messageType);
    inputMessage->FromBuffer(runner);
    Handleton::GetInstance<Logger>()->Log("Minion got: " + std::to_string(inputMessage->GetUID()) + " " +
                            std::to_string(inputMessage->GetOffset()) + " " + std::to_string(inputMessage->GetLength()), Logger::DEBUG);
    std::shared_ptr<ATaskArgs> createdArgs = Handleton::GetInstance<Factory<uint32_t, ATaskArgs, size_t, TaskArgs>>()->Create(messageType,
        inputMessage->GetUID(),
        TaskArgs(inputMessage->GetOffset(), inputMessage->GetLength(), inputMessage->GetData()));

    return createdArgs;
}

void MasterProxy::SendReadResponse(const UID& uid, bool result, size_t length, const std::shared_ptr<char[]>& data)
{
    MessageReadReceive rMsg(uid.GetID(), result, length, data);
    char* buffer = new char[rMsg.GetSize()];
    rMsg.ToBuffer(buffer);

    {
        std::unique_lock lock(m_mtx);
        m_socket.value().SendTo(buffer, rMsg.GetSize());
    }

    delete[] buffer;
}

void MasterProxy::SendWriteResponse(const UID& uid, bool result)
{
    MessageWriteReceive wMsg(uid.GetID(), result);
    char* buffer = new char[wMsg.GetSize()];
    wMsg.ToBuffer(buffer);

    {
        std::unique_lock lock(m_mtx);
        m_socket.value().SendTo(buffer, wMsg.GetSize());
    }
    
    delete[] buffer;
}

int MasterProxy::GetSocketFD()
{
    return m_socket.value().GetFD();
}