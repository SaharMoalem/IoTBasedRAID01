#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include "TcpClientSocket.hpp"

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
    int fds[2] = {-1, -1};
    TEST_ASSERT(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0);

    ilrd::TCPClient client(fds[0]);
    close(fds[1]);

    bool threw = false;
    try
    {
        char buf[8] = {};
        client.Receive(buf, sizeof(buf));
    }
    catch(const std::runtime_error&)
    {
        threw = true;
    }

    TEST_ASSERT(threw);
    std::cout << "tcp eof ok\n";
    return 0;
}
