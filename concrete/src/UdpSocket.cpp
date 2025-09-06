/******************************************************************************
 * File Name: UDPSocket
 * Owner: Sahar Moalem                                                             
 * Reviewer: Itai
 * Review status: Approved
 ******************************************************************************/ 
#include <iostream> // string, runtime_error
#include <netdb.h> // addrinfo, AI_PASSIVE, getaddrinfo, freeaddrinfo
#include <cstring> // memset
#include <unistd.h> //close

#include "UdpSocket.hpp"

using namespace std;

ilrd::UDPSocket::UDPSocket(const string& port, const char* ip_addr)
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

    if((socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype,
                                                servinfo->ai_protocol)) == -1)
    {
        throw runtime_error("failed to socket\n");
    }

    if(!ip_addr)
    {
        if(bind(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
        {
            close(socket_fd);
            throw runtime_error("failed to bind\n");
        }
    }

    m_serveraddr = *servinfo->ai_addr;
    freeaddrinfo(servinfo);
}

ilrd::UDPSocket::~UDPSocket()
{
    close(socket_fd);
}

void ilrd::UDPSocket::ReceiveFrom(char* buffer, size_t size)
{
    socklen_t addr_len = sizeof(m_serveraddr);
    ssize_t numbytes = recvfrom(socket_fd, buffer, size, 0, &m_serveraddr,
                                                                    &addr_len);

    if(numbytes == -1)
    {
        throw runtime_error("failed to recvfrom\n");
    }  
}

void ilrd::UDPSocket::SendTo(const char* message, size_t size)
{
    socklen_t addr_len = sizeof(m_serveraddr);

    if(sendto(socket_fd, message, size, 0, &m_serveraddr, addr_len) == -1)
    {
        throw runtime_error("failed to sendto\n");
    }
}

void ilrd::UDPSocket::EnableBroadcast()
{
    int broadcastPermission = 1;

    if(setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST,
                (void*)&broadcastPermission, sizeof(broadcastPermission)) == -1)
    {
        throw runtime_error("failed to enable broadcast\n");
    }
}

int ilrd::UDPSocket::GetFD()
{
    return socket_fd;
}