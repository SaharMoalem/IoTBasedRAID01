#include <iostream>
#include <arpa/inet.h>
#include <cstring>

#include "NBDProxy.hpp"
#include "factory.hpp"
#include "handleton.hpp"
#include "NBDReadArgs.hpp"
#include "NBDWriteArgs.hpp"
#include "logger.hpp"

using namespace ilrd;

NBDProxy::NBDProxy(NBD& nbd): m_nbd(nbd)
{
    Handleton::GetInstance<Factory<FDMODE, ATaskArgs, TaskArgs>>()->Register(FDMODE::READ,
        [](const TaskArgs& args) {
        return std::make_shared<NBDReadArgs>(args);
    });

    Handleton::GetInstance<Factory<FDMODE, ATaskArgs, TaskArgs>>()->Register(FDMODE::WRITE,
        [](const TaskArgs& args) {
        return std::make_shared<NBDWriteArgs>(args);
    });
}

std::shared_ptr<ITaskArgs> NBDProxy::GetTaskArgs(int fd, FDMODE mode)
{
    (void)fd;
    (void)mode;
    char buffer[sizeof(nbd_request)];

    {
        std::unique_lock lock(m_mtx);
        m_nbd.GetTCPSocket().Receive(buffer, sizeof(nbd_request));
    }

    const nbd_request request = *(nbd_request*)buffer;
    uint32_t length = ntohl(request.len);
    uint64_t offset = NBD::Ntohll(request.from);

    std::cout << length << " " << offset << std::endl;
    std::shared_ptr<char[]> data = nullptr;

    if(request.magic != htonl(NBD_REQUEST_MAGIC))
    {
        throw std::runtime_error("NBD_REQUEST_MAGIC failed");
    }

    switch(ntohl(request.type))
    {
        case NBD_CMD_READ:
            break;

        case NBD_CMD_WRITE:
            data.reset(new char[length]);

            {
                std::unique_lock lock(m_mtx);
                m_nbd.GetTCPSocket().Receive(data.get(), length);
            }

            break;

        default:
            throw std::runtime_error("Unknown request");
    }

    std::shared_ptr<ATaskArgs> createdArgs = Handleton::GetInstance<Factory<FDMODE,
            ATaskArgs, TaskArgs>>()->Create(!data ? READ : WRITE, TaskArgs(offset, length, data));
    nbd_reply reply;
    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.error = htonl(0);
    memcpy(reply.handle, request.handle, sizeof(reply.handle));
    
    {
        std::unique_lock lock(m_mtx);
        m_responses.emplace(createdArgs->GetUID(), reply);
    }

    m_dispatcher.Notify(createdArgs);

    return createdArgs;
}

void NBDProxy::RegisterForNewTaskArgs(ACallback<std::shared_ptr<ATaskArgs>>& callback)
{
    m_dispatcher.Register(&callback);   
}

void NBDProxy::OnNBDResponse(const std::shared_ptr<TaskResult>& result)
{
    nbd_reply reply;

    std::unique_lock lock(m_mtx);
    reply = m_responses[result->uid];
    m_responses.erase(result->uid);
    m_nbd.GetTCPSocket().Send((char*)&reply, sizeof(nbd_reply));

    if(result->buffer)
    {
        m_nbd.GetTCPSocket().Send(result->buffer.get(), result->length);
    }

    Handleton::GetInstance<Logger>()->Log("reply sent!", Logger::DEBUG);
}