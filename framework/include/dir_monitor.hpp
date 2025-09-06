#ifndef  __DIRMONITOR_HPP__
#define  __DIRMONITOR_HPP__

#include <string>
#include <thread>

#include "dispatcher.hpp"

namespace ilrd
{
class DirMonitor
{
public:
    DirMonitor(const std::string& path_name = "plugins/");
    ~DirMonitor();
    DirMonitor(const DirMonitor& other) = delete;
    DirMonitor& operator=(const DirMonitor& other) = delete;

    void RegisterOpen(ACallback<std::string>* plugin);
    void RegisterClose(ACallback<std::string>* plugin);
    void Run();

private:
    void MonitorRun();
    int GetNotifyFD();
    int GetWatchFD();

    Dispatcher<std::string> m_open_dispatcher;
    Dispatcher<std::string> m_close_dispatcher;
    std::thread m_worker;
    std::string m_dir_name;
    int m_notify_fd;
    int m_watchfd;
    bool m_running;
};

}

#endif