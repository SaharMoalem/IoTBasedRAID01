#include <unistd.h>     // STDIN_FILENO
#include <iostream>

#include "reactor.hpp"
#include "UdpSocket.hpp"

void TestReactor()
{
    ilrd::Reactor reactor(std::make_unique<ilrd::LinuxListener>());
    ilrd::UDPSocket udp("8080");
    reactor.Register(STDIN_FILENO, ilrd::IListener::READ, [&reactor](int, ilrd::IListener::Mode) { reactor.Stop(); });
    reactor.Register(udp.GetFD(), ilrd::IListener::READ,
        [&udp](int fd, ilrd::IListener::Mode mode)
        {
            static_cast<void>(mode);
            static_cast<void>(fd);
            char bytes[NAME_MAX];
            udp.ReceiveFrom(bytes, NAME_MAX);
            std::cout << bytes << std::endl;
        });

    reactor.Run();
}

int main()
{
    TestReactor();
    return 0;
}