#ifndef  __THREAD_POOL_HPP__
#define  __THREAD_POOL_HPP__

#include <map>      // map
#include <thread>   // jthread

#include "waitable_queue.hpp"
#include "tp_task.hpp"

namespace ilrd
{


class ThreadPool
{
public:
    enum Priority
    {
        LOW = 1,
        MEDIUM = 2,
        HIGH = 3
    };
    // Creating a new Thread Pool
    ThreadPool(size_t n_threads = std::thread::hardware_concurrency());

    // End Thread Pool execution
    ~ThreadPool();

    //Add a new task to Thread Pool to run
    void AddTask(std::shared_ptr<detail::ITPTask> s_ptr, Priority pr = LOW);

    //Set a new number of threads to run in the pool
    void SetNumThreads(size_t n_threads);

    //Pause Thread Pool execution
    void Pause();

    //Resume Thread Pool execution after calling Pause
    void Resume();

private:
    enum InnerPriority
    {
        EndPoolRun = 0,
        ILOW = 1,
        IMEDIUM = 2,
        IHIGH = 3,
        CRITICAL = 4
    };

    class TPTask
    {
    public:
        TPTask(std::shared_ptr<detail::ITPTask> task, InnerPriority pr = ILOW);
        bool operator<(const TPTask& other) const;
        void Run();

    private:
        std::shared_ptr<detail::ITPTask> m_task;
        InnerPriority m_priority;
        std::chrono::time_point<std::chrono::system_clock> m_time_created;
    };

    WaitablePQ<TPTask, std::priority_queue<TPTask>> m_pq;
    WaitableQueue<size_t> m_deleted_threads;
    std::map<size_t, std::jthread> m_threads;
    size_t m_total_threads;
    size_t m_tid;
    static thread_local bool m_running;
    static thread_local bool m_paused;

    std::mutex m_pause_mtx;
    std::condition_variable m_pause_cv;
    std::counting_semaphore<> m_pause_sem;
    
    void AddThreadsToPool(size_t amount);
    void RemoveThreadsFromPool(size_t amount, InnerPriority pr);
    static void ThreadRunTask(ThreadPool& pool, size_t tid);
    static void EndThreadTask();
    static void PauseThreadTask(ThreadPool& pool);
};
}

#endif