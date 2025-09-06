#include <iostream>
#include <cstdio>

void __attribute__((constructor)) Loaded()
{
    //std::cout << "test another dir monitor loaded" << std::endl;
    printf("test another dir monitor loaded\n");
}

void __attribute__((destructor)) Unloaded()
{
    //std::cout << "test another dir monitor unloaded" << std::endl;
    printf("test another dir monitor unloaded\n");
}