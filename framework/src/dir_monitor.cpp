#include <sys/inotify.h>
#include <stdexcept>        // runtime_error
#include <string>
#include <cstring>

#include "dir_monitor.hpp"

using namespace ilrd;

bool DirMonitor::IsSharedObject(const std::string& path)
{
    return path.size() >= 3 && path.compare(path.size() - 3, 3, ".so") == 0;
}

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
            if(!m_running)
            {
                return;
            }

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

            if((event->mask & IN_CLOSE_WRITE) && IsSharedObject(event->name))
            {
                m_open_dispatcher.Notify(m_dir_name + event->name);
            }
            else if((event->mask & IN_DELETE) && IsSharedObject(event->name))
            {
                m_close_dispatcher.Notify(m_dir_name + event->name);
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
    Stop();
}

void DirMonitor::Stop()
{
    if(!m_running && !m_worker.joinable())
    {
        return;
    }

    m_running = false;

    if(m_notify_fd != -1)
    {
        inotify_rm_watch(m_notify_fd, m_watchfd);
        close(m_notify_fd);
        m_notify_fd = -1;
    }

    if(m_worker.joinable())
    {
        m_worker.join();
    }
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