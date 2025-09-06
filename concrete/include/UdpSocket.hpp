#ifndef ILRD_RD1645_UDP_HPP
#define ILRD_RD1645_UDP_HPP

#include <sys/socket.h> //sockaddr
#include <string>

//#include "Socket.hpp"

namespace ilrd
{
    class UDPSocket//: public Socket
    {
        public:
            UDPSocket(const std::string& port, const char* ip_addr = nullptr);
            ~UDPSocket();
            void SendTo(const char* message, size_t size);
            //void SendTo(const std::string& message);
            void ReceiveFrom(char* buffer, size_t size);
            void EnableBroadcast();
            int GetFD();

        private:
            int socket_fd;
            sockaddr m_serveraddr;
    };
}

#endif