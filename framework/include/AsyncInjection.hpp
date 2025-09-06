#ifndef  __ASYNCINJECTION_HPP__
#define  __ASYNCINJECTION_HPP__

#include <functional>
#include <chrono>

#include "Ischedulertask.hpp"

namespace ilrd
{
using AsyncFunc = std::function<bool()>;

class AsyncInjection
{
public:
    AsyncInjection(AsyncFunc task, std::chrono::milliseconds interval);

private:
    ~AsyncInjection() = default;
    void PerformAction();

    class AsyncInjectionTask: public ISchedulerTask
    {
    public:
        AsyncInjectionTask(AsyncInjection& async);
        void Run() override;

    private:
        AsyncInjection& m_async;
    };

    AsyncFunc m_task;
    std::chrono::milliseconds m_interval;
};
}

#endif