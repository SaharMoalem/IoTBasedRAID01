#include "handleton.hpp"
#include "logger.hpp"

void ThreadFunc1()
{
    for(int i = 0; i < 10; ++i)
    {
        ilrd::Handleton::GetInstance<ilrd::Logger>()->Log("This is a test, can u handle it?",
                                                        ilrd::Logger::DEBUG);
    }
}

void ThreadFunc2()
{
    for(int i = 0; i < 10; ++i)
    {
        ilrd::Handleton::GetInstance<ilrd::Logger>()->Log("This is a test, u cant handle it",
                                                        ilrd::Logger::ERROR);
    }
}

void TestLogger()
{
    std::jthread one(ThreadFunc1);
    std::jthread two(ThreadFunc2);
}

int main()
{
    TestLogger();
    return 0;
}