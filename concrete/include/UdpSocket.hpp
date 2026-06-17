#ifndef ILRD_RD1645_UDP_HPP
#define ILRD_RD1645_UDP_HPP

#include <sys/socket.h> //sockaddr
#include <string>

#include "SocketBase.hpp"

namespace ilrd
{
    class UDPSocket : public SocketBase
    {
        public:
            UDPSocket(const std::string& port, const char* ip_addr = nullptr);
            ~UDPSocket() override = default;
            void SendTo(const char* message, size_t size);
            //void SendTo(const std::string& message);
            ssize_t ReceiveFrom(char* buffer, size_t size);
            void EnableBroadcast();

        private:
            sockaddr m_serveraddr;
    };
}

#endif