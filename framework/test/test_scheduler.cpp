// #include "scheduler.hpp"
// #include "scheduler_task.hpp"

// void PrintNumbers

// void TestScheduler()
// {
//     ilrd::SchedulerTask
// }

// int main()
// {
//     return 0;
// }

#include <iostream>

#include "scheduler.hpp"

namespace tasks
{

class PrintTask : public ilrd::ISchedulerTask
{
public:
    PrintTask(int i);
    void Run() override;

private:
    int m_i;
};

PrintTask::PrintTask(int i) : m_i(i)
{ }

void PrintTask::Run()
{
    std::cout << "i = " << m_i << std::endl;
}

}   // namespace tasks


void TestsForScheduler();

int main()
{
    TestsForScheduler();

    return 0;
}

void TestsForScheduler()
{
    size_t n = 5;
    size_t ms = 10000;
    ilrd::Scheduler* scheduler = ilrd::Handleton::GetInstance<ilrd::Scheduler>();
    
    for (size_t i = 1; i <= n; ++i, ms -= 2000)
    {
        scheduler->AddTask(std::make_shared<tasks::PrintTask>(i),
                            std::chrono::milliseconds(ms));
    }

}
