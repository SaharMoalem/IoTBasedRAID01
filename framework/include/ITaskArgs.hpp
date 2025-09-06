#ifndef  __ITASKARGS_HPP__
#define  __ITASKARGS_HPP__

namespace ilrd
{
class ITaskArgs
{
public:
    virtual int GetKey() const = 0;
};
}

#endif