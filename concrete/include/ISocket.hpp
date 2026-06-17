#ifndef ILRD_RD1645_ISOCKET_HPP
#define ILRD_RD1645_ISOCKET_HPP

namespace ilrd
{
class ISocket
{
public:
    virtual ~ISocket() = default;
    virtual int GetFD() const = 0;
};
}

#endif

