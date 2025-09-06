#ifndef  __FRAMEWORK_HPP__
#define  __FRAMEWORK_HPP__

#include "reactor.hpp"
#include "dir_monitor.hpp"
#include "dll_loader.hpp"
#include "ICommand.hpp"
#include "IInputProxy.hpp"

namespace ilrd
{

class Framework
{
public:
    using Fd_Callbacks = std::vector<std::tuple<int, IListener::Mode, std::shared_ptr<IInputProxy>>>;
    using Creators = std::vector<std::pair<int, std::function<std::shared_ptr<ICommand>()>>>;

    Framework(const Fd_Callbacks& fd_callbacks, const Creators& creators,
                            const std::string& plugins_dir_path = "plugins/");
    ~Framework() = default;
    Framework(const Framework& other) = delete;
    Framework& operator=(const Framework& other) = delete;

    void Run();

private:
    const Fd_Callbacks& m_fd_callbacks;
    DirMonitor m_monitor;
    Reactor m_reactor;
    DLLLoader m_loader;
    Callback<std::string, DLLLoader> m_open_callback;
    Callback<std::string, DLLLoader> m_close_callback;
};
}

#endif