#ifndef  ILRD_RD1645_COMMANDS_HPP
#define  ILRD_RD1645_COMMANDS_HPP

#include "ICommand.hpp"

namespace ilrd
{
class ReadCommand: public ICommand
{
public:
    ReadCommand() = default;
    ~ReadCommand() = default;

    std::optional<std::pair<ilrd::AsyncFunc, std::chrono::milliseconds>> Run(std::shared_ptr<ilrd::ITaskArgs> args);
};

class WriteCommand: public ICommand
{
public:
    WriteCommand() = default;
    ~WriteCommand() = default;

    std::optional<std::pair<ilrd::AsyncFunc, std::chrono::milliseconds>> Run(std::shared_ptr<ilrd::ITaskArgs> args);
};
}

#endif