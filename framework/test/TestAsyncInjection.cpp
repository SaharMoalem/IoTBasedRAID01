#include <iostream>

#include "AsyncInjection.hpp"

int counter = 0;

bool Task()
{
    std::cout << counter << std::endl;
    return ++counter == 5;
}

int main()
{
    std::function<bool()> func = Task;
    ilrd::AsyncInjection::Schedule(func, std::chrono::milliseconds(1000));
}