#include <iostream>

#include "handleton.hpp"

extern "C" void PrintAnotherAddress()
{
    std::cout << ilrd::Handleton::GetInstance<int>() << std::endl;
}