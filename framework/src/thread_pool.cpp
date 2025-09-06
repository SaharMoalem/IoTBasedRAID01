#include "thread_pool.hpp"

using namespace ilrd;
using namespace detail;

/*********************************static variables*****************************/

thread_local bool ThreadPool::m_running = true;
thread_local bool ThreadPool::m_paused = false;

/*************************ThreadPool Special Tasks*****************************/

void ThreadPool::ThreadRunTask(ThreadPool& pool, size_t tid)
{
    while(m_running)
    {
        std::shared_ptr<ITPTask> task_ptr;
        TPTask tp_task(task_ptr);
        pool.m_pq.Pop(tp_task);
        tp_task.Run();
    }

    pool.m_deleted_threads.Push(tid);
}

void ThreadPool::PauseThreadTask(ThreadPool& pool)
{
    std::unique_lock<std::mutex> lock(pool.m_pause_mtx);
    pool.m_pause_sem.release();
    pool.m_pause_cv.wait(lock, [&pool] {return !pool.m_paused;});
}

void ThreadPool::EndThreadTask()
{
    m_running = false;
}

/*************************ThreadPool Private Functions*************************/

void ThreadPool::AddThreadsToPool(size_t amount)
{
    std::function<void(ThreadPool&, size_t)> init_task = ThreadRunTask;
    size_t curr_idx = 0;

    while(!m_deleted_threads.IsEmpty() && amount > 0)
    {
        m_deleted_threads.Pop(curr_idx);
        m_threads.at(curr_idx) = std::jthread(init_task, std::ref(*this),
                                                                    curr_idx);
        --amount;
    }

    for(size_t i = 0 ; i < amount; ++i, ++m_tid)
    {
        m_threads.emplace(m_tid, std::jthread(init_task, std::ref(*this),
                                                                        m_tid));
    }
}

void ThreadPool::RemoveThreadsFromPool(size_t amount, InnerPriority pr)
{
    std::function<void()> func = EndThreadTask;
    std::shared_ptr<ITPTask> stop_threads(new FunctionTask<>(func));

    for(size_t i = 0; i < amount; ++i)
    {
        m_pq.Push(TPTask(stop_threads, pr));
    }
}

/*************************TPTask functions*************************************/

ThreadPool::TPTask::TPTask(std::shared_ptr<ITPTask> task, InnerPriority pr):
    m_task(task), m_priority(pr),
    m_time_created(std::chrono::system_clock::now()) {}

bool ThreadPool::TPTask::operator<(const TPTask& other) const
{
    return (m_time_created < other.m_time_created) *
            (m_priority == other.m_priority) + (m_priority < other.m_priority);
}

void ThreadPool::TPTask::Run()
{
    m_task->Run();
}

/*************************ThreadPool functions*********************************/

ThreadPool::ThreadPool(size_t n_threads):
        m_total_threads(n_threads), m_tid(0), m_pause_sem({0})
{
    AddThreadsToPool(m_total_threads);
}

ThreadPool::~ThreadPool()
{
    RemoveThreadsFromPool(m_total_threads, EndPoolRun);
}

void ThreadPool::AddTask(std::shared_ptr<ITPTask> s_ptr, Priority pr)
{
    m_pq.Push(TPTask(s_ptr, static_cast<InnerPriority>(pr)));
}

void ThreadPool::Pause()
{
    m_paused = true;
    std::function<void(ThreadPool&)> func = PauseThreadTask;
    std::shared_ptr<ITPTask> pause_thread(new FunctionTask<ThreadPool&>(func,
                                                                        *this));

    for(size_t i = 0; i < m_total_threads; ++i)
    {
        m_pq.Push(TPTask(pause_thread, CRITICAL));
    }

    for(size_t i = 0; i < m_total_threads; ++i)
    {
        m_pause_sem.acquire();
    }
}

void ThreadPool::Resume()
{
    std::unique_lock<std::mutex> lock(m_pause_mtx);
    m_paused = false;
    m_pause_cv.notify_all();
}

void ThreadPool::SetNumThreads(size_t n_threads)
{
    if(n_threads > m_total_threads)
    {
        AddThreadsToPool(n_threads - m_total_threads);
    }

    else
    {
        RemoveThreadsFromPool(m_total_threads - n_threads, CRITICAL);
    }

    m_total_threads = n_threads;
}