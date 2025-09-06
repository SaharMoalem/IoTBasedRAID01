#ifndef  __ILISTENER_HPP__
#define  __ILISTENER_HPP__

#include <vector>

namespace ilrd
{
class IListener
{
public:
    enum Mode {
        WRITE,
        READ
    };

    virtual std::vector<std::pair<int, Mode>> Listen(std::vector<std::pair<int, Mode>> fd_list) = 0;
};
}

#endif