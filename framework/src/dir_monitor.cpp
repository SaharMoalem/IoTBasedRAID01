#include <sys/inotify.h>
#include <stdexcept>        // runtime_error
#include <string>
#include <cstring>

#include "dir_monitor.hpp"

using namespace ilrd;

/*************************Private Functions************************************/

int DirMonitor::GetNotifyFD()
{
    const int fd = inotify_init();

    if(fd == -1)
    {
        throw std::runtime_error("Couldn't init inotify");
    }

    return fd;
}

int DirMonitor::GetWatchFD()
{
    const int watch = inotify_add_watch(m_notify_fd, m_dir_name.c_str(),
                                                    IN_CLOSE_WRITE | IN_DELETE);
    if(watch == -1)
    {
        close(m_notify_fd);
        throw std::runtime_error("Couldn't init watch");
    }

    return watch;
}

void DirMonitor::MonitorRun()
{
    char buffer[sizeof(inotify_event) + NAME_MAX + 1];

    while(m_running)
    {
        ssize_t size = read(m_notify_fd, buffer, sizeof(buffer));

        if(size == -1)
        {
            throw std::runtime_error("Error reading events");
        }

        if(size == 0)
        {
            continue;
        }

        char* ptr = buffer;
        while(ptr < buffer + size)
        {
            const inotify_event* event = reinterpret_cast<inotify_event*>(ptr);

            if(event->mask & IN_CLOSE_WRITE)
            {
                m_open_dispatcher.Notify(m_dir_name +
                    std::string(event->name, event->len));
            }
            else if(event->mask & IN_DELETE)
            {
                m_close_dispatcher.Notify(m_dir_name +
                    std::string(event->name, event->len));
            }

            ptr += sizeof(inotify_event) + event->len;
        }
    }
}

/*****************************API Functions************************************/

DirMonitor::DirMonitor(const std::string& path_name): m_dir_name(path_name),
    m_notify_fd(GetNotifyFD()), m_watchfd(GetWatchFD()), m_running(false) {}

DirMonitor::~DirMonitor()
{
    m_running = false;

    inotify_rm_watch(m_notify_fd, m_watchfd);
    
    if(m_worker.joinable())
    {
        m_worker.join();
    }

    close(m_notify_fd);
}

void DirMonitor::RegisterOpen(ACallback<std::string>* plugin)
{
    m_open_dispatcher.Register(plugin);
}

void DirMonitor::RegisterClose(ACallback<std::string>* plugin)
{
    m_close_dispatcher.Register(plugin);
}

void DirMonitor::Run()
{
    m_running = true;
    m_worker = std::thread([this] {MonitorRun();});
}