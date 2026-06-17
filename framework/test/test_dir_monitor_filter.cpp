#include <iostream>

#include "test_macros.h"
#include "dir_monitor.hpp"

int main()
{
    TEST_ASSERT(ilrd::DirMonitor::IsSharedObject("plugin.so"));
    TEST_ASSERT(ilrd::DirMonitor::IsSharedObject("/tmp/libfoo.so"));
    TEST_ASSERT(!ilrd::DirMonitor::IsSharedObject("plugin.txt"));
    TEST_ASSERT(!ilrd::DirMonitor::IsSharedObject("so"));

    std::cout << "dir monitor filter ok\n";
    return 0;
}
