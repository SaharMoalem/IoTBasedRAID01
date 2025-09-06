#ifndef  __SCHEDULER_HPP__
#define  __SCHEDULER_HPP__

#include <queue>
#include <chrono>
#include <csignal>
#include <condition_variable>

#include "Ischedulertask.hpp"
#include "handleton.hpp"

namespace ilrd
{
class Scheduler
{
public:
    ~Scheduler();
    Scheduler(const Scheduler& other) = delete;
    Scheduler& operator=(const Scheduler& other) = delete;

    void AddTask(const std::shared_ptr<ISchedulerTask>& task,
                                    const std::chrono::milliseconds& interval);

private:
    friend class Handleton;
    Scheduler();
    void StartTimer();
    static void ThreadFunc(sigval sval);
    void RunTask();

    class Task
    {
    public:
        explicit Task(const std::shared_ptr<ISchedulerTask>& task = nullptr,
            const std::chrono::milliseconds& interval = std::chrono::milliseconds(0));
        void Run();
        bool operator<(const Task& other) const;
        std::chrono::time_point<std::chrono::system_clock> GetTime();

    private:
        std::shared_ptr<ISchedulerTask> m_task;
        std::chrono::time_point<std::chrono::system_clock> m_time_to_run;
    };

    std::priority_queue<Task> m_queue;
    std::mutex m_mtx;
    size_t m_counter;
    std::condition_variable m_cv;
    timer_t m_timer;
    itimerspec m_its;
};
}

#endif