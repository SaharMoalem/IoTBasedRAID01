#include <iostream>

#include "Framework.hpp"
#include "handleton.hpp"
#include "factory.hpp"
#include "thread_pool.hpp"
#include "FW_TPTask.hpp"

using namespace ilrd;

Framework::Framework(const Fd_Callbacks& fd_callbacks, const Creators& creators,
                                        const std::string& plugins_dir_path):
        m_fd_callbacks(fd_callbacks), m_monitor(plugins_dir_path),
        m_open_callback((&DLLLoader::Load), m_loader),
        m_close_callback((&DLLLoader::Unload), m_loader)
{    
    for(const auto& [fd, mode, callback] : m_fd_callbacks)
    {
        m_reactor.Register(fd, mode,
            [callback](int fd, IListener::Mode mode)
            {
                std::shared_ptr<ITaskArgs> pTaskArgs = callback->GetTaskArgs(fd, static_cast<FDMODE>(mode));
                
                if(pTaskArgs.get() == nullptr)
                {
                    throw std::runtime_error("failed to get args.");
                }

                std::shared_ptr<FW_TPTask> fw_task = std::make_shared<FW_TPTask>(pTaskArgs);
                Handleton::GetInstance<ThreadPool>()->AddTask(fw_task);
            });
    }

    for(const auto& creator : creators)
    {
        Handleton::GetInstance<Factory<int, ICommand>>()->Register(creator.first, creator.second);
    }

    m_monitor.RegisterOpen(&m_open_callback);
    m_monitor.RegisterClose(&m_close_callback);
}

void Framework::Run()
{
    m_monitor.Run();
    m_reactor.Run();
}