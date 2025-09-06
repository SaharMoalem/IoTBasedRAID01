#ifndef  ILRD_RD1645_IMINIONPROXY_HPP
#define  ILRD_RD1645_IMINIONPROXY_HPP

#include <cstddef>
#include <memory>

#include "AMessage.hpp"
#include "dispatcher.hpp"

namespace ilrd
{
class IMinionProxy
{
public:
    virtual void AddWriteTask(const UID& uid, size_t offset, size_t length, const std::shared_ptr<char[]>& data, std::function<void(const std::shared_ptr<AMessage>&)> onDone) = 0;
    virtual void AddReadTask(const UID& uid, size_t offset, size_t length, std::function<void(const std::shared_ptr<AMessage>&)> onDone) = 0;
    virtual int GetMinionFD() = 0;
    virtual void OnMinionFDWakeUp() = 0;
};
}

#endif