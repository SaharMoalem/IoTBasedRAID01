#include "AsyncInjection.hpp"
#include "scheduler.hpp"

using namespace ilrd;

AsyncInjection::AsyncInjectionTask::AsyncInjectionTask(AsyncInjection& async):
                                                            m_async(async) {}

void AsyncInjection::AsyncInjectionTask::Run()
{
    m_async.PerformAction();
}

AsyncInjection::AsyncInjection(AsyncFunc task,
    std::chrono::milliseconds interval): m_task(task), m_interval(interval)
{
    Handleton::GetInstance<Scheduler>()->AddTask(std::make_shared<AsyncInjectionTask>(*this), m_interval);
}

void AsyncInjection::PerformAction()
{
    if(m_task())
    {
        delete this;
    }

    else
    {
        Handleton::GetInstance<Scheduler>()->AddTask(std::make_shared<AsyncInjectionTask>(*this), m_interval);
    }
}