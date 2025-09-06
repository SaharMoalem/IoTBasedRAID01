#include "reactor.hpp"

using namespace ilrd;

Reactor::Reactor(std::unique_ptr<IListener> listener):
    m_listener(std::move(listener)), m_running(false)
{
}

void Reactor::Register(int fd, IListener::Mode mode,
                                std::function<void(int, IListener::Mode)> func)
{
    m_listen_funcs.emplace(std::make_pair(fd, mode), func); 
}

void Reactor::Unregister(int fd, IListener::Mode mode)
{
    m_listen_funcs.erase(std::make_pair(fd, mode));
}

void Reactor::Run()
{
    m_running = true;

    while(m_running)
    {
        std::vector<std::pair<int, IListener::Mode>> fd_list;

        for(const auto& pair : m_listen_funcs)
        {
            fd_list.push_back(std::make_pair(pair.first.first, pair.first.second));
        }

        std::vector<std::pair<int, IListener::Mode>> selected_fds = m_listener->Listen(fd_list);

        for(const auto& pair : selected_fds)
        {
            m_listen_funcs.at(std::make_pair(pair.first, pair.second))(pair.first, pair.second);
        }
    }
}

void Reactor::Stop()
{
    m_running = false;
}