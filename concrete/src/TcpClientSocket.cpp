/******************************************************************************
 * File Name: TCPClientSocket
 * Owner: Sahar Moalem                                                             
 * Reviewer: Itai
 * Review status: Approved
 ******************************************************************************/ 
#include <cstring>      //memset
#include <netdb.h>      //addrinfo, AI_PASSIVE, getaddrinfo, freeaddrinfo
#include <unistd.h>     //close
#include <iostream>     // string, runtime_error

#include "TcpClientSocket.hpp"

using namespace std;

ilrd::TCPClient::TCPClient(const string& other_port, const string& other_ip)
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

    if((socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype,
                                                servinfo->ai_protocol)) == -1)
    {
        throw runtime_error("failed to socket\n");
    }

    if(connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        throw runtime_error("failed to connect\n");
    }

    freeaddrinfo(servinfo);
}

ilrd::TCPClient::TCPClient(int fd): socket_fd(fd) {}

ilrd::TCPClient::~TCPClient()
{
    close(socket_fd);
}

void ilrd::TCPClient::Send(const char* buffer, size_t size)
{
    ssize_t numbytes;

    while(size > 0)
    {
        numbytes = send(socket_fd, buffer, size, 0);

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
        numbytes = recv(socket_fd, buffer, size, 0);

        if(numbytes == -1)
        {
            throw std::runtime_error("failed to receive\n");
        }

        buffer += numbytes;
        size -= numbytes;
    }

    return size;
}

int ilrd::TCPClient::GetFD()
{
    return socket_fd;
}