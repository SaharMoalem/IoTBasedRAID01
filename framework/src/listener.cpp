#include <iostream>

#include "linux_listener.hpp"

using namespace ilrd;

std::vector<std::pair<int, IListener::Mode>> LinuxListener::Listen(std::vector<std::pair<int, Mode>> fd_list)
{
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    int max_fd = 0;

    for(const auto& pair : fd_list)
    {
        FD_SET(pair.first, (pair.second == READ) ? &read_fds : &write_fds);
        max_fd = max_fd * (max_fd > pair.first) + pair.first *
                                                        (max_fd <= pair.first);
    }

    if(select(max_fd + 1, &read_fds, &write_fds, nullptr, nullptr) < 0)
    {
        throw std::runtime_error("select failed");
    }

    std::vector<std::pair<int, Mode>> selected_fds;

    for(const auto& pair : fd_list)
    {
        if(FD_ISSET(pair.first, &read_fds) || FD_ISSET(pair.first, &write_fds))
        {
            selected_fds.push_back(std::make_pair(pair.first, pair.second));
        }
    }

    return selected_fds;
}