#include <iostream>
#include <thread>
#include <string>

#include "singleton.hpp"

void ThreadFunc1()
{
    size_t i = 200;
    while (i-- != 0)
    {
        try
        {
            std::string* const str =  ilrd::Singleton<std::string>::GetInstance();
            str->append("a");
        }
        catch (const std::runtime_error& e)
        {
            std::cout << e.what() << std::endl;
            break;
        }
    }
}

void ThreadFunc2()
{
    size_t i = 0;
    while (i < 20)
    {
        std::cout << *ilrd::Singleton<std::string>::GetInstance() << std::endl;
        ++i;
    }
}

void TestSingleton()
{
    std::string* const instance = ilrd::Singleton<std::string>::GetInstance();
    ilrd::Singleton<std::string>::GetInstance()->append("TestSingleton");
    std::cout << ilrd::Singleton<std::string>::GetInstance()->c_str() << std::endl;
    *instance = "Blabla";
    std::cout << *ilrd::Singleton<std::string>::GetInstance() << std::endl;
    std::jthread one(ThreadFunc1);
    std::jthread two(ThreadFunc2);
}

int main()
{
    TestSingleton();
    return 0;
}
