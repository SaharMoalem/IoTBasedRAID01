#ifndef  ILRD_RD1645_MINIONCOMMANDS_HPP
#define  ILRD_RD1645_MINIONCOMMANDS_HPP

#include "ICommand.hpp"

namespace ilrd
{
class MinionReadCommand: public ICommand
{
public:
    MinionReadCommand() = default;
    ~MinionReadCommand() = default;

    std::optional<std::pair<ilrd::AsyncFunc, std::chrono::milliseconds>> Run(std::shared_ptr<ilrd::ITaskArgs> args);
};

class MinionWriteCommand: public ICommand
{
public:
    MinionWriteCommand() = default;
    ~MinionWriteCommand() = default;

    std::optional<std::pair<ilrd::AsyncFunc, std::chrono::milliseconds>> Run(std::shared_ptr<ilrd::ITaskArgs> args);
};
}

#endif