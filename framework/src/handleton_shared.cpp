#include <iostream>

#include "handleton.hpp"

extern "C" void PrintAddress()
{
    std::cout << ilrd::Handleton::GetInstance<int>() << std::endl;
}