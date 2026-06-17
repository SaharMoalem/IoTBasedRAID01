#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "UdpSocket.hpp"

#define TEST_ASSERT(cond)                                                      \
    do                                                                         \
    {                                                                          \
        if(!(cond))                                                            \
        {                                                                      \
            std::cerr << "FAIL: " << #cond << " at " << __FILE__ << ":"        \
                      << __LINE__ << "\n";                                     \
            return 1;                                                          \
        }                                                                      \
    } while(0)

int main()
{
    ilrd::UDPSocket server("19090");
    ilrd::UDPSocket client("19090", "127.0.0.1");

    const char msg[] = "hello-udp";
    client.SendTo(msg, sizeof(msg));

    char buffer[64] = {};
    const ssize_t bytes = server.ReceiveFrom(buffer, sizeof(buffer));
    TEST_ASSERT(bytes == static_cast<ssize_t>(sizeof(msg)));
    TEST_ASSERT(std::memcmp(buffer, msg, sizeof(msg)) == 0);

    std::cout << "udp receive ok\n";
    return 0;
}
