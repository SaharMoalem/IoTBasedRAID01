#include <iostream>

#include "ICommand.hpp"
#include "handleton.hpp"
#include "factory.hpp"

using namespace ilrd;

class UpdatedReadCommand : public ICommand
{
public:
    UpdatedReadCommand() = default;
    ~UpdatedReadCommand() = default;

    std::optional<std::pair<AsyncFunc, std::chrono::milliseconds>> Run(std::shared_ptr<ITaskArgs> args)
    {
        std::cout << "Updated" << std::endl;
        return {};
    }
};

std::shared_ptr<ilrd::ICommand> CreateUpdatedReadCommand()
{
    return std::make_shared<UpdatedReadCommand>();
}

void __attribute__((constructor)) SoMain()
{
    Handleton::GetInstance<Factory<int, ICommand>>()->Register(0, CreateUpdatedReadCommand);
}

void __attribute__((destructor)) SoExit()
{
    std::cout << "plugin removed\n";
}