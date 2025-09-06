#include "dir_monitor.hpp"
#include "dll_loader.hpp"

using namespace ilrd;

void TestDirMonitor()
{
    DirMonitor monitor;
    DLLLoader loader;
    Callback<std::string, DLLLoader> open_callback((&DLLLoader::Load), loader);
    Callback<std::string, DLLLoader> close_callback((&DLLLoader::Unload), loader);
    monitor.RegisterOpen(&open_callback);
    monitor.RegisterClose(&close_callback);
    monitor.Run();
    //while(true);
    sleep(20);
}

int main()
{
    TestDirMonitor();
    return 0;
}