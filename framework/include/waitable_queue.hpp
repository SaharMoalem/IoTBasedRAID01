#ifndef  __WAITABLEQ_HPP__
#define  __WAITABLEQ_HPP__

#include <queue>                // queue, priority_queue
#include <mutex>                // mutex
#include <condition_variable>   // condition_variable
#include <chrono>               //chrono

namespace ilrd
{

/**************************static functions************************************/
template<typename T, typename U>
static void Enqueue(T& queue, const U& value, std::timed_mutex& mtx,
                                                    std::condition_variable_any& cv)
{
    {
        std::unique_lock<std::timed_mutex> lock(mtx);
        queue.push(value);
    }
    
    cv.notify_one();
}

template<typename T>
static bool IsEmptyHelper(T& queue, std::timed_mutex& mtx)
{
    std::unique_lock<std::timed_mutex> lock(mtx);
    return queue.empty();
}

/*****************************WaitableQueue Class******************************/

template<typename T, class Q = std::queue<T>>

// Waitable Thread-Safe templated Regular Queue
class WaitableQueue
{
public:
    // pushing value to queue - O(1)
    void Push(const T& value);

    // popping value from queue and save it in output - O(1)
    void Pop(T& output);

    // popping value from queue under given timeout ms and save it in output (true)
    // if thread waited for more then timeout ms, nothing will happen (false)
    // O(1)
    bool Pop(T& out, std::chrono::milliseconds timeout);

    // check whether the queue is empty or not - O(1)
    bool IsEmpty() const;

private:
    alignas(64) Q queue;
    alignas(64) mutable std::timed_mutex m_mutex;
    alignas(64) std::condition_variable_any m_cv;
};

template<typename T, class Q>
void WaitableQueue<T, Q>::Push(const T& value)
{
    Enqueue(queue, value, m_mutex, m_cv);
}

template<typename T, class Q>
void WaitableQueue<T, Q>::Pop(T& output)
{
    std::unique_lock<std::timed_mutex> lock(m_mutex);
    m_cv.wait(lock, [this] { return !queue.empty(); });
    output = queue.front();
    queue.pop();
}

template<typename T, class Q>
bool WaitableQueue<T, Q>::Pop(T& output, std::chrono::milliseconds timeout)
{
    std::unique_lock<std::timed_mutex> lock(m_mutex);

    if(m_cv.wait_for(lock, timeout, [this] { return !queue.empty(); }))
    {
        output = queue.front();
        queue.pop();
        return true;
    }

    return false;
}

template<typename T, class Q>
bool WaitableQueue<T, Q>::IsEmpty() const
{
    return IsEmptyHelper(queue, m_mutex);
}

/*****************************WaitablePQ Class*********************************/

template<typename T, class Q = std::priority_queue<T>>

// Waitable Thread-Safe templated Priority Queue
class WaitablePQ
{
public:
    // pushing value to pq - O(1)
    void Push(const T& value);

    // popping value from pq and save it in output - O(1)
    void Pop(T& output);

    // popping value from pq under given timeout ms and save it in output (true)
    // if thread waited for more then timeout ms, nothing will happen (false)
    // O(1)
    bool Pop(T& out, std::chrono::milliseconds timeout);

    // check whether the pq is empty or not - O(1)
    bool IsEmpty() const;

private:
    alignas(64) Q queue;
    alignas(64) mutable std::timed_mutex m_mutex;
    alignas(64) std::condition_variable_any m_cv;
};

template<typename T, class Q>
void WaitablePQ<T, Q>::Push(const T& value)
{
    Enqueue(queue, value, m_mutex, m_cv);
}

template<typename T, class Q>
void WaitablePQ<T, Q>::Pop(T& output)
{
    std::unique_lock<std::timed_mutex> lock(m_mutex);
    m_cv.wait(lock, [this]{ return !queue.empty(); });
    output = queue.top();
    queue.pop();
}

template<typename T, class Q>
bool WaitablePQ<T, Q>::Pop(T& output, std::chrono::milliseconds timeout)
{
    std::unique_lock<std::timed_mutex> lock(m_mutex);

    if(m_cv.wait_for(lock, timeout, [this]{ return !queue.empty(); }))
    {
        output = queue.top();
        queue.pop();
        return true;
    }

    return false;
}

template<typename T, class Q>
bool WaitablePQ<T, Q>::IsEmpty() const
{
    return IsEmptyHelper(queue, m_mutex);
}

}

#endif