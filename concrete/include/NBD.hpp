#ifndef ILRD_RD1645_NBD_HPP
#define ILRD_RD1645_NBD_HPP

#include <cstdint>
#include <sys/types.h>
#include <optional>

#include "TcpClientSocket.hpp"

namespace ilrd
{
class NBD
{
public:
    explicit NBD(const char* device, uint64_t driveSize);
    ~NBD();

    TCPClient& GetTCPSocket();
    static uint64_t Ntohll(uint64_t num);

private:
    class FDWrapper
    {
    public:
        explicit FDWrapper(int fd);
        ~FDWrapper();
        
        int GetFD() const;

    private:
        int m_fd;
    };

    void SocketsInit();
    void RunClient();
    static void DisconnectNBD(int signal);

    FDWrapper m_device;
    std::optional<FDWrapper> m_clientSocket;
    std::optional<TCPClient> m_serverSocket;
    uint64_t m_driveSize;
    pid_t m_childPid;
    static int s_nbdDevToDisconnect;
};
}


#endif
