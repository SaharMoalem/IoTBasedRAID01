#ifndef  __IINPUTPROXY_HPP__
#define  __IINPUTPROXY_HPP__

#include <memory>

#include "ITaskArgs.hpp"

namespace ilrd
{
enum FDMODE {
        WRITE,
        READ
    };

class IInputProxy
{
public:
    virtual std::shared_ptr<ITaskArgs> GetTaskArgs(int fd, FDMODE mode) = 0;
};
}

#endif