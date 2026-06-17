#include "Commands.hpp"
#include "handleton.hpp"
#include "MinionManager.hpp"
#include "NBDReadArgs.hpp"
#include "NBDWriteArgs.hpp"
#include "logger.hpp"

using namespace ilrd;

std::optional<std::pair<AsyncFunc, std::chrono::milliseconds>> ReadCommand::Run(std::shared_ptr<ITaskArgs> args)
{
    NBDReadArgs* readArgs = dynamic_cast<NBDReadArgs*>(args.get());
    if(!readArgs)
    {
        Handleton::GetInstance<Logger>()->Log(
            "ReadCommand: invalid task args", Logger::ERROR);
        return std::nullopt;
    }

    try
    {
        Handleton::GetInstance<MinionManager>()->AddReadTask(readArgs->GetUID(),
            readArgs->GetOffset(), readArgs->GetLength());
    }
    catch(const std::exception& e)
    {
        Handleton::GetInstance<Logger>()->Log(
            std::string("ReadCommand: ") + e.what(), Logger::ERROR);
    }

    return std::nullopt;
}

std::optional<std::pair<AsyncFunc, std::chrono::milliseconds>> WriteCommand::Run(std::shared_ptr<ITaskArgs> args)
{
    NBDWriteArgs* writeArgs = dynamic_cast<NBDWriteArgs*>(args.get());
    if(!writeArgs)
    {
        Handleton::GetInstance<Logger>()->Log(
            "WriteCommand: invalid task args", Logger::ERROR);
        return std::nullopt;
    }

    try
    {
        Handleton::GetInstance<MinionManager>()->AddWriteTask(writeArgs->GetUID(),
            writeArgs->GetOffset(), writeArgs->GetLength(), writeArgs->GetBuffer());
    }
    catch(const std::exception& e)
    {
        Handleton::GetInstance<Logger>()->Log(
            std::string("WriteCommand: ") + e.what(), Logger::ERROR);
    }

    return std::nullopt;
}
