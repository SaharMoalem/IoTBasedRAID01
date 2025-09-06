#include <iostream>

#include "Response.hpp"

using namespace ilrd;

void Response::OnSuccess()
{
    std::cout << "Task Success!\n";
}

void Response::OnFailure()
{
    std::cout << "Task Failed\n";
}