#ifndef  ILRD_RD1645_MINIONPROXY_HPP
#define  ILRD_RD1645_MINIONPROXY_HPP

#include <map>

#include "IMinionProxy.hpp"
#include "UdpSocket.hpp"

namespace ilrd
{
class MinionProxy: public IMinionProxy
{
public:
    MinionProxy(const std::string& ip, const std::string& port);
    void AddWriteTask(const UID& uid, size_t offset, size_t length,
        const std::shared_ptr<char[]>& data,
        std::function<void(const std::shared_ptr<AMessage>&)> onDone) override;
    void AddReadTask(const UID& uid, size_t offset, size_t length,
        std::function<void(const std::shared_ptr<AMessage>&)> onDone) override;
    int GetMinionFD() override;
    void OnMinionFDWakeUp() override;
    
private:
    UDPSocket m_socket;
    std::map<UID, std::function<void(const std::shared_ptr<AMessage>&)>> m_map;
    std::mutex m_mtx;
};
}

#endif