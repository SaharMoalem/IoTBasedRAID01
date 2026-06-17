#include "scheduler.hpp"

using namespace ilrd;

/*************************Task Functions***************************************/

Scheduler::Task::Task(const std::shared_ptr<ISchedulerTask>& task,
                    const std::chrono::milliseconds& interval): m_task(task),
                m_time_to_run(std::chrono::system_clock::now() + interval) {}

void Scheduler::Task::Run()
{
    m_task->Run();
}

bool Scheduler::Task::operator<(const Task& other) const
{
    return m_time_to_run > other.m_time_to_run;
}

std::chrono::time_point<std::chrono::system_clock> Scheduler::Task::GetTime()
{
    return m_time_to_run;
}

/****************************Scheduler Functions*******************************/

Scheduler::Scheduler(): m_counter(0), m_its()
{
    sigevent sev{};
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = ThreadFunc;
    sev.sigev_value.sival_ptr = this;

    if(timer_create(CLOCK_REALTIME, &sev, &m_timer) != 0)
    {
        throw std::runtime_error("Failed to create timer");
    }
}

Scheduler::~Scheduler()
{
    itimerspec disarm{};
    timer_settime(m_timer, 0, &disarm, nullptr);
    timer_delete(m_timer);

    std::unique_lock lock(m_mtx);
    while(!m_queue.empty())
    {
        m_queue.pop();
    }
    m_counter = 0;
    m_cv.notify_all();
}

void Scheduler::AddTask(const std::shared_ptr<ISchedulerTask>& task,
                                    const std::chrono::milliseconds& interval)
{
    std::unique_lock lock(m_mtx);
    m_queue.emplace(task, interval);
    ++m_counter;
    StartTimer();
}

void Scheduler::StartTimer()
{
    Task curr_task = m_queue.top();
    m_its.it_value.tv_sec = std::chrono::system_clock::to_time_t(curr_task.GetTime());
    const auto remainder = curr_task.GetTime() -
        std::chrono::system_clock::from_time_t(m_its.it_value.tv_sec);
    m_its.it_value.tv_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(
        remainder).count();

    if(timer_settime(m_timer, TIMER_ABSTIME, &m_its, nullptr) !=  0)
    {
        throw std::runtime_error("failed to set timer");
    }
}

void Scheduler::ThreadFunc(sigval sval)
{
    Scheduler* sched = static_cast<Scheduler*>(sval.sival_ptr);
    sched->RunTask(); 
}

void Scheduler::RunTask()
{
    Task curr_task;

    {
        std::unique_lock lock(m_mtx);
        if(m_queue.empty())
        {
            return;
        }

        curr_task = m_queue.top();
        m_queue.pop();
    }

    try
    {
        curr_task.Run();
    }
    catch(...)
    {
    }

    std::unique_lock lock(m_mtx);

    if(--m_counter == 0)
    {
        m_cv.notify_one();
    }
    else if(!m_queue.empty())
    {
        StartTimer();
    }
}