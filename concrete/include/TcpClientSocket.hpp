#ifndef ILRD_RD1645_TCPClient_HPP
#define ILRD_RD1645_TCPClient_HPP

#include <string>
#include <sys/types.h>

namespace ilrd
{
    class TCPClient//: public Socket
    {
        public:
            TCPClient(const std::string& other_port,
                                                const std::string& other_ip);
            explicit TCPClient(int fd);
            ~TCPClient();

            void Send(const char* buffer, size_t size);
            ssize_t Receive(char* buffer, size_t size);
            int GetFD();

        private:
            int socket_fd;
    };
}
#endif