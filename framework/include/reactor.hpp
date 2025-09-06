#ifndef  __REACTOR_HPP__
#define  __REACTOR_HPP__

#include <functional>
#include <memory>
#include <map>

#include "linux_listener.hpp"

namespace ilrd
{
class Reactor
{
public:
    Reactor(std::unique_ptr<IListener> listener = std::make_unique<LinuxListener>());
    ~Reactor() = default;
    Reactor(const Reactor& other) = delete;
    Reactor& operator=(const Reactor& other) = delete;

    void Register(int fd, IListener::Mode mode, std::function<void(int, IListener::Mode)> func);
    void Unregister(int fd, IListener::Mode mode);
    void Run();
    void Stop();

private:
    std::unique_ptr<IListener> m_listener;
    std::map<std::pair<int, IListener::Mode>, std::function<void(int, IListener::Mode)>> m_listen_funcs;
    bool m_running;
};
}

#endif