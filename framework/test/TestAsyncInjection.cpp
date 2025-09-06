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
    new ilrd::AsyncInjection(func, std::chrono::milliseconds(1000));
}