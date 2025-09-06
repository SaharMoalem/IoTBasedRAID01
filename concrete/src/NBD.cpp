#define _POSIX_C_SOURCE (200809L)

#include <fcntl.h>      
#include <linux/nbd.h>
#include <netinet/in.h>
#include <csignal>
#include <sys/ioctl.h>

#include "handleton.hpp"
#include "logger.hpp"

#include "NBD.hpp"

using namespace ilrd;

NBD::FDWrapper::FDWrapper(int fd): m_fd(fd)
{
    if(m_fd == -1)
    {
        throw std::runtime_error("FDWrapper failed");
    }
}

NBD::FDWrapper::~FDWrapper()
{
    close(m_fd);
}

int NBD::FDWrapper::GetFD() const
{
    return m_fd;
}

int NBD::s_nbdDevToDisconnect = -1;

const uint64_t NBD::s_driveSize = 1024 * 1024 * 4 * 3;

#ifdef WORDS_BIGENDIAN
uint64_t NBD::Ntohll(uint64_t num)
{
    return num;
}
#else
uint64_t NBD::Ntohll(uint64_t num)
{
    uint32_t lo = num & 0xffffffff;
    uint32_t hi = num >> 32U;
    lo = ntohl(lo);
    hi = ntohl(hi);

    return static_cast<uint64_t>(lo) << 32U | hi;
}
#endif

NBD::NBD(const char* device): m_device(open(device, O_RDWR))
{
    SocketsInit();
    pid_t pid = fork();

    if(pid == -1)
    {
        throw std::runtime_error("fork failed");
    }

    if(!pid)
    {
        RunClient();
    }

    s_nbdDevToDisconnect = m_device.GetFD();
    struct sigaction act;
    act.sa_handler = DisconnectNBD;
    act.sa_flags = SA_RESTART;

    if (sigemptyset(&act.sa_mask) || sigaddset(&act.sa_mask, SIGINT) ||
                                            sigaddset(&act.sa_mask, SIGTERM))
    {
        throw std::runtime_error("failed to prepare signal mask in server");
    }

    if (sigaction(SIGINT, &act, nullptr) || sigaction(SIGTERM, &act, nullptr))
    {
        throw std::runtime_error("failed to set server signals");
    }
}

NBD::~NBD()
{
    DisconnectNBD(0);
}

void NBD::RunClient()
{
    sigset_t sigset;
    int deviceFD = m_device.GetFD();

    if(sigfillset(&sigset) || sigprocmask(SIG_SETMASK, &sigset, nullptr))
    {
        throw std::runtime_error("failed to block signals in client");
    }

    if(ioctl(deviceFD, NBD_SET_SOCK, m_clientSocket.value().GetFD()) == -1)
    {
        throw std::runtime_error("ioctl NBD_SET_SOCK failed");
    }

    if(ioctl(deviceFD, NBD_DO_IT) == -1)//block
    {
        throw std::runtime_error("ioctl NBD_DO_IT failed");
    }

    if(ioctl(deviceFD, NBD_CLEAR_QUE) == -1)
    {
        throw std::runtime_error("ioctl NBD_CLEAR_QUE failed");
    }

    if(ioctl(deviceFD, NBD_CLEAR_SOCK) == -1)
    {
        throw std::runtime_error("ioctl client NBD_CLEAR_SOCK failed");
    }
}

void NBD::SocketsInit()
{
    int sockets[2];

    if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets))
    {
        throw std::runtime_error("socketpair failed");
    }

    m_serverSocket.emplace(sockets[0]);
    m_clientSocket.emplace(sockets[1]);

    if(ioctl(m_device.GetFD(), NBD_SET_SIZE, s_driveSize) == -1)
    {
        throw std::runtime_error("ioctl NBD_SET_SIZE failed");
    }

    if(ioctl(m_device.GetFD(), NBD_CLEAR_SOCK) == -1)
    {
        throw std::runtime_error("ioctl NBD_CLEAR_SOCK failed");
    }
}

TCPClient& NBD::GetTCPSocket()
{
    return m_serverSocket.value();
}

void NBD::DisconnectNBD(int signal)
{
    (void)signal;

    if(s_nbdDevToDisconnect != -1)
    {
        if(ioctl(s_nbdDevToDisconnect, NBD_DISCONNECT) == -1)
        {
            Handleton::GetInstance<Logger>()->Log("failed to request disconect on nbd device", Logger::WARNING);
        }

        else
        {
            s_nbdDevToDisconnect = -1;
            Handleton::GetInstance<Logger>()->Log("sucessfuly requested disconnect on nbd device", Logger::DEBUG);
        }
    }
}
