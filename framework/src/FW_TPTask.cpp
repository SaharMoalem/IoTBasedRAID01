#include "FW_TPTask.hpp"
#include "ICommand.hpp"
#include "handleton.hpp"
#include "factory.hpp"
#include "logger.hpp"

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
            AsyncInjection::Schedule(async.value().first, async.value().second);
        }
    }
    
    catch(const std::exception& e)
    {
        Handleton::GetInstance<Logger>()->Log(
            std::string("FW_TPTask: ") + e.what(), Logger::ERROR);
    }
    catch(...)
    {
        Handleton::GetInstance<Logger>()->Log(
            "FW_TPTask: unknown exception", Logger::ERROR);
    }
}