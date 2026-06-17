#ifndef  __ASYNCINJECTION_HPP__
#define  __ASYNCINJECTION_HPP__

#include <functional>
#include <chrono>
#include <memory>

#include "Ischedulertask.hpp"

namespace ilrd
{
using AsyncFunc = std::function<bool()>;

class AsyncInjection : public std::enable_shared_from_this<AsyncInjection>
{
public:
    static void Schedule(AsyncFunc task, std::chrono::milliseconds interval);

private:
    AsyncInjection(AsyncFunc task, std::chrono::milliseconds interval);
    void PerformAction();
    void Reschedule();

    class AsyncInjectionTask: public ISchedulerTask
    {
    public:
        explicit AsyncInjectionTask(const std::shared_ptr<AsyncInjection>& async);
        void Run() override;

    private:
        std::weak_ptr<AsyncInjection> m_async;
    };

    AsyncFunc m_task;
    std::chrono::milliseconds m_interval;
};
}

#endif
