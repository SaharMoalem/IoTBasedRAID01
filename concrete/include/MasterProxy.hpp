#ifndef  ILRD_RD1645_MASTERPROXY_HPP
#define  ILRD_RD1645_MASTERPROXY_HPP

#include <string>
#include <map>
#include <optional>

#include "IInputProxy.hpp"
#include "handleton.hpp"
#include "UdpSocket.hpp"
#include "MessageSend.hpp"
#include "MessageReceive.hpp"


namespace ilrd
{
class MasterProxy: public IInputProxy
{
public:
    ~MasterProxy() = default;

    void Init(const std::string& port);
    std::shared_ptr<ITaskArgs> GetTaskArgs(int fd, FDMODE mode) override;
    void SendReadResponse(const UID& uid, bool result, size_t length, const std::shared_ptr<char[]>& data);
    void SendWriteResponse(const UID& uid, bool result);
    int GetSocketFD();
    
private:
    friend class Handleton;
    MasterProxy() = default;
    std::optional<UDPSocket> m_socket;
    std::mutex m_mtx;
};
}

#endif