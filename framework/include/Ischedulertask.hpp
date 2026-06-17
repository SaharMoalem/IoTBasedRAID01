#ifndef  __ISCHEDULERTASK_HPP__
#define  __ISCHEDULERTASK_HPP__

namespace ilrd
{
class ISchedulerTask
{
public:
    virtual ~ISchedulerTask() = default;
    virtual void Run() = 0;
};
}

#endif