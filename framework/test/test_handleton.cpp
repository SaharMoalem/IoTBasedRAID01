#include <iostream>
#include <dlfcn.h>

#include "handleton.hpp"

void TestHandleton()
{
    void* handle1 = dlopen("lib1.so", RTLD_LAZY);
    void* handle2 = dlopen("lib2.so", RTLD_LAZY);

    void* func1 = dlsym(handle1, "PrintAddress");
    void* func2 = dlsym(handle2, "PrintAnotherAddress");

    reinterpret_cast<void (*)()>(func1)();
    reinterpret_cast<void (*)()>(func2)();
    std::cout << ilrd::Handleton::GetInstance<int>() << std::endl;


    dlclose(handle1);
    dlclose(handle2);
}

int main()
{
    TestHandleton();

    return 0;
}
