// #include <iostream>

// #include "thread_pool.hpp"

// using namespace ilrd;
// using namespace std;

// void FunctionTaskTest1()
// {
//     cout << "Hello MEDIUM!\n"; 
// }

// void FunctionTaskTest2()
// {
//     cout << "Hello HIGH!\n"; 
// }

// void TestSimplePool()
// {
//     std::function<void()> func1 = FunctionTaskTest1;
//     std::function<void()> func2 = FunctionTaskTest2;
//     shared_ptr<ITPTask> task1(new FunctionTask(func1));
//     shared_ptr<ITPTask> task2(new FunctionTask(func2));

//     ThreadPool pool(10);
    
//     pool.Pause();
//     cout << "Paused.\n";
//     pool.AddTask(task1, MEDIUM);
//     pool.AddTask(task2, HIGH);
//     pool.Resume();

//     std::this_thread::sleep_for(std::chrono::seconds(5));
// }

// int main()
// {
//     TestSimplePool();

//     return 0;
// }
// Filename: ThreadPoolTest.cpp
// Created by shiningblack on 3/9/25.
// Reviewer:
// Review Status:

#include <iostream>
#include <random>

#include "test_macros.h"
#include "thread_pool.hpp"

static int Task1(const int num)
{
    (void)sleep(2U);
    return num;
}

static void Task2()
{
    std::cout << "I'm doing Task2" << std::endl;
    (void)sleep(3U);
    std::cout << "I'm done with Task2" << std::endl;
}

void BasicTest()
{
    ilrd::ThreadPool pool(4U);
    int resArr[8U] = {};
    std::shared_ptr<ilrd::detail::FutureFunction<int, int>> futures[8U];
    for (size_t i = 0U; i < 8U; ++i)
    {
        futures[i] = std::make_shared<ilrd::detail::FutureFunction<int, int>>(Task1, resArr[i], 1);
    }
    for (const auto & future : futures)
    {
        pool.AddTask(future);
    }
    for (const auto & future : futures)
    {
        std::cout << future->Get() << std::endl;
    }
}

void TestPauseResume()
{
    ilrd::ThreadPool pool(4U);
    const auto task = std::make_shared<ilrd::detail::FunctionTask<>>(Task2);
    std::cout << "Start Pause Resume" << std::endl;

    (void)pool.Pause();

    for (size_t i = 0U; i < 8U; ++i)
    {
        pool.AddTask(task);
    }

    (void)sleep(3U);

    (void)pool.Resume();

    //(void)pool.Pause();
}

void TestResize()
{
    ilrd::ThreadPool pool(4U);
    const auto task = std::make_shared<ilrd::detail::FunctionTask<>>(Task2);
    std::cout << "Start RESIZE test" << std::endl;

    for (size_t i = 0U; i < 20U; ++i)
    {
        pool.AddTask(task);
    }

    pool.SetNumThreads(2U);

    (void)sleep(10U);

    for (size_t i = 0U; i < 40U; ++i)
    {
        pool.AddTask(task);
    }

    (void)pool.Pause();
    std::cout << "PAUSED" << std::endl;
    pool.SetNumThreads(6U);

    (void)sleep(10U);
    (void)pool.Resume();
}


void EvilTest()
{
    ilrd::ThreadPool pool(0U);
    int num;
    const auto task = std::make_shared<ilrd::detail::FunctionTask<>>(Task2);
    const auto task2 = std::make_shared<ilrd::detail::FutureFunction<int, int>>(Task1, num, 1);
    std::cout << RED"EVIL LAIR BOSS STAGE" << std::endl;

    (void)pool.Pause();
    (void)pool.Pause();
    (void)pool.Pause();
    (void)pool.Resume();

    pool.SetNumThreads(1U);
    (void)pool.Pause();
    pool.SetNumThreads(5U);
    pool.SetNumThreads(2U);
    pool.SetNumThreads(99U);
    pool.SetNumThreads(4U);
    (void)pool.Resume();
    (void)pool.Resume();
    (void)pool.Resume();
    (void)pool.Pause();
    (void)pool.Pause();
    (void)pool.Resume();

    for (size_t i = 0U; i < 20U; ++i)
    {
        pool.AddTask(task);
    }

    (void)pool.Pause();
    pool.SetNumThreads(2U);
    (void)pool.Resume();
    pool.AddTask(task2, ilrd::ThreadPool::Priority::HIGH);
    pool.AddTask(task2, ilrd::ThreadPool::Priority::LOW);
    (void)task2->Get();
    std::cout << "Got result" << std::endl;
    --num;
    pool.SetNumThreads(4U);
    (void)task2->Get();
    std::cout << "Got result" << std::endl;

    for (size_t i = 0U; i < 10U; ++i)
    {
        pool.AddTask(task);
    }

    (void)pool.Pause();
    pool.SetNumThreads(0U);
    (void)pool.Resume();
    pool.SetNumThreads(5U);
    pool.SetNumThreads(5U);
    (void)pool.Pause();
    std::cout << GREEN"Le finale" << std::endl;
}

int main()
{
    BasicTest();
    TestPauseResume();
    TestResize();
    EvilTest();
    PASS;
    return 0;
}
