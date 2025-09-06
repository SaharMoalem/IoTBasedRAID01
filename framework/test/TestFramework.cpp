#include <iostream>

#include "Framework.hpp"
#include "NBDProxy.hpp"
#include "handleton.hpp"
#include "MinionProxy.hpp"
#include "MinionManager.hpp"
#include "ResponseManager.hpp"
#include "Commands.hpp"
#include "NBD.hpp"

std::shared_ptr<ilrd::ICommand> CreateReadCommand()
{
    return std::make_shared<ilrd::ReadCommand>();
}

std::shared_ptr<ilrd::ICommand> CreateWriteCommand()
{
    return std::make_shared<ilrd::WriteCommand>();
}

void TestFramework()
{
    ilrd::NBD nbd("/dev/nbd4");
    ilrd::NBDProxy nbd_proxy(nbd);
    std::vector<std::shared_ptr<ilrd::IMinionProxy>> minions;
    
    minions.push_back(std::make_shared<ilrd::MinionProxy>("10.1.0.228", "9090"));
    minions.push_back(std::make_shared<ilrd::MinionProxy>("127.0.0.1", "9091"));
    minions.push_back(std::make_shared<ilrd::MinionProxy>("127.0.0.1", "9092"));

    ilrd::Handleton::GetInstance<ilrd::MinionManager>()->Init(4 * 1024 * 1024, minions);
    ilrd::Handleton::GetInstance<ilrd::ResponseManager>()->Init(nbd_proxy);

    ilrd::Framework::Fd_Callbacks fd_callbacks;
    fd_callbacks.push_back(std::make_tuple(nbd.GetTCPSocket().GetFD(),
        ilrd::IListener::READ, std::shared_ptr<ilrd::NBDProxy>(&nbd_proxy)));

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