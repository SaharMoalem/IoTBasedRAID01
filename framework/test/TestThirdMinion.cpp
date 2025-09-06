#include <iostream>

#include "Framework.hpp"
#include "MasterProxy.hpp"
#include "handleton.hpp"
#include "MinionCommands.hpp"
#include "FileManager.hpp"

std::shared_ptr<ilrd::ICommand> CreateReadCommand()
{
    return std::make_shared<ilrd::MinionReadCommand>();
}

std::shared_ptr<ilrd::ICommand> CreateWriteCommand()
{
    return std::make_shared<ilrd::MinionWriteCommand>();
}

void TestFramework()
{
    std::shared_ptr<ilrd::MasterProxy> proxy(ilrd::Handleton::GetInstance<ilrd::MasterProxy>());

    proxy->Init("9092");

    ilrd::Handleton::GetInstance<ilrd::FileManager>()->Init("./Minion3.dat");

    ilrd::Framework::Fd_Callbacks fd_callbacks;
    fd_callbacks.push_back(std::make_tuple(proxy->GetSocketFD(), ilrd::IListener::READ,
                                                    proxy));
    ilrd::Framework::Creators creators;
    std::function<std::shared_ptr<ilrd::ICommand>()> f_read = CreateReadCommand;
    std::function<std::shared_ptr<ilrd::ICommand>()> f_write = CreateWriteCommand;

    creators.push_back(std::make_pair(0, f_write));
    creators.push_back(std::make_pair(1, f_read));

    ilrd::Framework fw(fd_callbacks, creators);
    fw.Run();
}

int main()
{
    TestFramework();
    return 0;
}