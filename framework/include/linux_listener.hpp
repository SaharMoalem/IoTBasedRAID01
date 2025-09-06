#ifndef  __LINUX_LISTENER_HPP__
#define  __LINUX_LISTENER_HPP__

#include "Ilistener.hpp"

namespace ilrd
{
class LinuxListener: public IListener
{
public:
    LinuxListener() = default;
    ~LinuxListener() = default;
    std::vector<std::pair<int, IListener::Mode>> Listen(std::vector<std::pair<int, IListener::Mode>> fd_list);
};
}

#endif