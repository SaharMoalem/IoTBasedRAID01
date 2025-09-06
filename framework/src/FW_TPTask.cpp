#include "FW_TPTask.hpp"
#include "ICommand.hpp"
#include "handleton.hpp"
#include "factory.hpp"

using namespace ilrd;

FW_TPTask::FW_TPTask(std::shared_ptr<ITaskArgs> task_arg): m_pTaskArg(task_arg)
{
}

void FW_TPTask::Run()
{
    try
    {
        std::shared_ptr<ICommand> pCommand = Handleton::GetInstance<Factory<int, ICommand>>()->Create(m_pTaskArg->GetKey());
        std::optional<std::pair<AsyncFunc, std::chrono::milliseconds>> async = pCommand->Run(m_pTaskArg);

        if(async)
        {   
            new AsyncInjection(async.value().first, async.value().second);
        }
    }
    
    catch(...)
    {
        
    }
}