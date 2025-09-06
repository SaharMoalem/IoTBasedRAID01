#ifndef  __ICOMMAND_HPP__
#define  __ICOMMAND_HPP__

#include <optional>             // optional
#include <memory>

#include "AsyncInjection.hpp"
#include "ITaskArgs.hpp"

namespace ilrd
{
class ICommand
{
public:
    virtual std::optional<std::pair<AsyncFunc, std::chrono::milliseconds>> Run(std::shared_ptr<ITaskArgs> args) = 0;
};
}

#endif