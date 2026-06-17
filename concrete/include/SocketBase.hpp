#ifndef ILRD_RD1645_SOCKET_BASE_HPP
#define ILRD_RD1645_SOCKET_BASE_HPP

#include <unistd.h> // close

#include "ISocket.hpp"

namespace ilrd
{
class SocketBase : public ISocket
{
public:
    SocketBase(const SocketBase&) = delete;
    SocketBase& operator=(const SocketBase&) = delete;

    SocketBase(SocketBase&& other) noexcept : m_fd(other.m_fd)
    {
        other.m_fd = -1;
    }

    SocketBase& operator=(SocketBase&& other) noexcept
    {
        if(this != &other)
        {
            Reset(other.m_fd);
            other.m_fd = -1;
        }
        return *this;
    }

    ~SocketBase() override
    {
        Reset(-1);
    }

    int GetFD() const override
    {
        return m_fd;
    }

protected:
    explicit SocketBase(int fd) : m_fd(fd) {}

    void Reset(int fd)
    {
        if(m_fd != -1)
        {
            close(m_fd);
        }
        m_fd = fd;
    }

private:
    int m_fd;
};
}

#endif

