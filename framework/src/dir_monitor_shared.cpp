#include <iostream>

void __attribute__((constructor)) Loaded()
{
    std::cout << "test dir monitor loaded" << std::endl;
}

void __attribute__((destructor)) Unloaded()
{
    std::cout << "test dir monitor unloaded" << std::endl;
}