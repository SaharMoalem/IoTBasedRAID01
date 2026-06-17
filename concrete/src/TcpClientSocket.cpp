#include <cstring>      //memset
#include <cerrno>
#include <netdb.h>      //addrinfo, AI_PASSIVE, getaddrinfo, freeaddrinfo
#include <unistd.h>     //close
#include <iostream>     // string, runtime_error

#include "TcpClientSocket.hpp"

using namespace std;

ilrd::TCPClient::TCPClient(const string& other_port, const string& other_ip)
: SocketBase(-1)
{
    addrinfo addr, *servinfo;

    memset(&addr, 0, sizeof(addr));
    addr.ai_family = AF_INET;
    addr.ai_socktype = SOCK_STREAM;
    addr.ai_flags = AI_PASSIVE;

    if(getaddrinfo(other_ip.c_str(), other_port.c_str(), &addr, &servinfo) != 0)
    {
        throw runtime_error("failed to getaddrinfo\n");
    }

    int fd = socket(servinfo->ai_family, servinfo->ai_socktype,
                                                servinfo->ai_protocol);
    if(fd == -1)
    {
        throw runtime_error("failed to socket\n");
    }

    if(connect(fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        close(fd);
        throw runtime_error("failed to connect\n");
    }

    Reset(fd);
    freeaddrinfo(servinfo);
}

ilrd::TCPClient::TCPClient(int fd): SocketBase(fd) {}

void ilrd::TCPClient::Send(const char* buffer, size_t size)
{
    ssize_t numbytes;

    while(size > 0)
    {
        numbytes = send(GetFD(), buffer, size, 0);

        if(numbytes <= 0)
        {
            throw runtime_error("failed to send\n");
        }

        buffer += numbytes;
        size -= numbytes;
    }
}

ssize_t ilrd::TCPClient::Receive(char* buffer, size_t size)
{
    ssize_t numbytes;

    while(size > 0)
    {
        numbytes = recv(GetFD(), buffer, size, 0);

        if(numbytes == 0)
        {
            throw std::runtime_error("disconnected while receiving\n");
        }

        if(numbytes == -1)
        {
            if(errno == EINTR)
            {
                continue;
            }
            throw std::runtime_error("failed to receive\n");
        }

        buffer += numbytes;
        size -= numbytes;
    }

    return size;
}
