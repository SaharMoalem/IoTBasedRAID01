#include <iostream>
#include <thread>
#include <chrono>

#include "test_macros.h"
#include "thread_pool.hpp"

int main()
{
    {
        ilrd::ThreadPool pool(4);
        auto task = std::make_shared<ilrd::detail::FunctionTask<>>([]() {});
        for(size_t i = 0; i < 8; ++i)
        {
            pool.AddTask(task);
        }
    }

    std::cout << "thread pool shutdown ok\n";
    return 0;
}
