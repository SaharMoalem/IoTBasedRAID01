#include "AsyncInjection.hpp"
#include "scheduler.hpp"
#include "handleton.hpp"

using namespace ilrd;

AsyncInjection::AsyncInjectionTask::AsyncInjectionTask(
    const std::shared_ptr<AsyncInjection>& async): m_async(async) {}

void AsyncInjection::AsyncInjectionTask::Run()
{
    if(std::shared_ptr<AsyncInjection> injection = m_async.lock())
    {
        injection->PerformAction();
    }
}

void AsyncInjection::Schedule(AsyncFunc task, std::chrono::milliseconds interval)
{
    std::shared_ptr<AsyncInjection> injection(
        new AsyncInjection(std::move(task), interval));
    Handleton::GetInstance<Scheduler>()->AddTask(
        std::make_shared<AsyncInjectionTask>(injection), interval);
}

AsyncInjection::AsyncInjection(AsyncFunc task,
    std::chrono::milliseconds interval): m_task(std::move(task)),
    m_interval(interval) {}

void AsyncInjection::PerformAction()
{
    if(m_task())
    {
        return;
    }

    Reschedule();
}

void AsyncInjection::Reschedule()
{
    Handleton::GetInstance<Scheduler>()->AddTask(
        std::make_shared<AsyncInjectionTask>(shared_from_this()), m_interval);
}
