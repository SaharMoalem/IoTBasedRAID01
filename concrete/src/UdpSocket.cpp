#include <iostream> // string, runtime_error
#include <netdb.h> // addrinfo, AI_PASSIVE, getaddrinfo, freeaddrinfo
#include <cstring> // memset
#include <unistd.h> //close

#include "UdpSocket.hpp"

using namespace std;

ilrd::UDPSocket::UDPSocket(const string& port, const char* ip_addr)
: SocketBase(-1)
{
    addrinfo *servinfo, addr;

    memset(&addr, 0, sizeof(addr));
    addr.ai_family = AF_INET;
    addr.ai_socktype = SOCK_DGRAM;
    addr.ai_flags = AI_PASSIVE;

    if(getaddrinfo(ip_addr, port.c_str(), &addr, &servinfo) != 0)
    {
        throw runtime_error("failed to getaddrinfo\n");
    }

    int fd = socket(servinfo->ai_family, servinfo->ai_socktype,
                                                servinfo->ai_protocol);
    if(fd == -1)
    {
        throw runtime_error("failed to socket\n");
    }

    if(!ip_addr)
    {
        if(bind(fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
        {
            close(fd);
            throw runtime_error("failed to bind\n");
        }
    }

    m_serveraddr = *servinfo->ai_addr;
    Reset(fd);
    freeaddrinfo(servinfo);
}

ssize_t ilrd::UDPSocket::ReceiveFrom(char* buffer, size_t size)
{
    socklen_t addr_len = sizeof(m_serveraddr);
    ssize_t numbytes = recvfrom(GetFD(), buffer, size, 0, &m_serveraddr,
                                                                    &addr_len);

    if(numbytes == -1)
    {
        throw runtime_error("failed to recvfrom\n");
    }

    return numbytes;
}

void ilrd::UDPSocket::SendTo(const char* message, size_t size)
{
    socklen_t addr_len = sizeof(m_serveraddr);

    if(sendto(GetFD(), message, size, 0, &m_serveraddr, addr_len) == -1)
    {
        throw runtime_error("failed to sendto\n");
    }
}

void ilrd::UDPSocket::EnableBroadcast()
{
    int broadcastPermission = 1;

    if(setsockopt(GetFD(), SOL_SOCKET, SO_BROADCAST,
                (void*)&broadcastPermission, sizeof(broadcastPermission)) == -1)
    {
        throw runtime_error("failed to enable broadcast\n");
    }
}
