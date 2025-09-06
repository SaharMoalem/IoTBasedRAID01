#include <cstddef>        
#include <string>             
#include <iostream>            
#include <vector>               
#include <thread>               

#include "test_macros.h"        
#include "waitable_queue.hpp"    

class Task
{
public:
    Task(std::string name = "", int priority = 1);
    std::string GetName() const;
    int GetPriority() const;
    friend std::ostream& operator<<(std::ostream& os, const Task& task);

private:
    int m_priority;
    std::string m_name;
};

struct TaskComparator
{
    bool operator()(const Task& task1, const Task& task2)
    {
        return task1.GetPriority() < task2.GetPriority();
    }
};

using namespace ilrd;

Task::Task(std::string name, int priority): m_priority(priority), m_name(name)
{ }

std::string Task::GetName() const
{
    return m_name;
}

int Task::GetPriority() const
{
    return m_priority;
}

std::ostream& operator<<(std::ostream& os, const Task& task)
{
    return (os << "Task name: " << task.m_name << std::endl <<
            "Task Priority: " << task.m_priority << std::endl);
}

void ProducerThread(WaitableQueue<int>& queue, size_t iterations, int seconds)
{
    int k = 1;

    for (size_t i = 0; i < iterations; ++i, ++k)
    {
        queue.Push(k);
        std::cout << "Pushed: " << k << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }
}

void ConsumerBlockingThread(WaitableQueue<int>& queue, size_t iterations)
{
    int element = 0;

    for (size_t i = 0; i < iterations; ++i)
    {
        queue.Pop(element);
        std::cout << "Popped: " << element << std::endl;
    }
}

void BasicTestsWaitableQueue()
{
    size_t element;
    WaitableQueue<size_t> queue;
    
    queue.Push(4);
    TEST("IsEmpty check", queue.IsEmpty(), false);
    queue.Push(6);
    TEST("IsEmpty check", queue.IsEmpty(), false);

    queue.Pop(element, std::chrono::milliseconds(3000));
    TEST("non-blocking (timeout) pop check", element, 4);
    queue.Pop(element, std::chrono::milliseconds(3000));
    TEST("non-blocking (timeout) pop check", element, 6);

    queue.Pop(element, std::chrono::milliseconds(3000));

    queue.Push(5);
    queue.Push(6);
    queue.Push(7);
    queue.Pop(element);
    TEST("blocking pop check", element, 5);
    queue.Pop(element);
    TEST("blocking pop check", element, 6);
    queue.Pop(element);
    TEST("blocking pop check", element, 7);

    TEST("IsEmpty check", queue.IsEmpty(), true);
}

void TestOneProducerAndConsumer()
{
    WaitableQueue<int> queue;
    const size_t iterations = 5;

    TEST("sanity check", queue.IsEmpty(), true);

    std::thread producer(ProducerThread, std::ref(queue), iterations, 2);
    std::thread consumer(ConsumerBlockingThread, std::ref(queue), iterations);

    producer.join();
    consumer.join();

    TEST("sanity check", queue.IsEmpty(), true);
}

void TestOneProducerMultipleConsumers()
{
    WaitableQueue<int> queue;
    const size_t iterations = 3;
    std::thread consumers[iterations];
    std::thread producer(ProducerThread, std::ref(queue), iterations, 1);
    
    for (size_t i = 0; i < iterations; ++i)
    {
        consumers[i] = std::thread(ConsumerBlockingThread, std::ref(queue), 1);
    }

    producer.join();

    for (size_t i = 0; i < iterations; ++i)
    {
        consumers[i].join();
    }
}

void TestMultipleProducersAndConsumers()
{
    WaitableQueue<int> queue;
    const size_t iterations = 4;
    std::thread producers[iterations];
    std::thread consumers[iterations];

    for (size_t i = 0; i < iterations; ++i)
    {
        producers[i] = std::thread(ProducerThread, std::ref(queue), 1, 1);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        consumers[i] = std::thread(ConsumerBlockingThread, std::ref(queue), 1);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (size_t i = 0; i < iterations; ++i)
    {
        producers[i].join();
        consumers[i].join();
    }
}

int main()
{
    BasicTestsWaitableQueue();
    TestOneProducerAndConsumer();
    TestOneProducerMultipleConsumers();
    TestMultipleProducersAndConsumers();

    PASS;

    return 0;
}