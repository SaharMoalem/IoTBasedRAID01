#include "MinionCommands.hpp"
#include "MinionArgs.hpp"
#include "handleton.hpp"
#include "FileManager.hpp"
#include "MasterProxy.hpp"
#include "logger.hpp"

using namespace ilrd;

std::optional<std::pair<ilrd::AsyncFunc, std::chrono::milliseconds>> MinionReadCommand::Run(std::shared_ptr<ilrd::ITaskArgs> args)
{
    MinionReadArgs* readArgs = dynamic_cast<MinionReadArgs*>(args.get());
    if(!readArgs)
    {
        Handleton::GetInstance<Logger>()->Log(
            "MinionReadCommand: invalid task args", Logger::ERROR);
        return std::nullopt;
    }

    try
    {
        std::shared_ptr<char[]> readBuffer(new char[readArgs->GetLength()]);
        const bool result = Handleton::GetInstance<FileManager>()->ReadFromFile(
            readArgs->GetOffset(), readArgs->GetLength(), readBuffer);
        Handleton::GetInstance<MasterProxy>()->SendReadResponse(readArgs->GetUID(),
            result, readArgs->GetLength(), readBuffer);
    }
    catch(const std::exception& e)
    {
        Handleton::GetInstance<Logger>()->Log(
            std::string("MinionReadCommand: ") + e.what(), Logger::ERROR);
    }

    return std::nullopt;
}

std::optional<std::pair<ilrd::AsyncFunc, std::chrono::milliseconds>> MinionWriteCommand::Run(std::shared_ptr<ilrd::ITaskArgs> args)
{
    MinionWriteArgs* writeArgs = dynamic_cast<MinionWriteArgs*>(args.get());
    if(!writeArgs)
    {
        Handleton::GetInstance<Logger>()->Log(
            "MinionWriteCommand: invalid task args", Logger::ERROR);
        return std::nullopt;
    }

    try
    {
        const bool result = Handleton::GetInstance<FileManager>()->WriteToFile(
            writeArgs->GetOffset(), writeArgs->GetLength(), writeArgs->GetBuffer());
        Handleton::GetInstance<MasterProxy>()->SendWriteResponse(
            writeArgs->GetUID(), result);
    }
    catch(const std::exception& e)
    {
        Handleton::GetInstance<Logger>()->Log(
            std::string("MinionWriteCommand: ") + e.what(), Logger::ERROR);
    }

    return std::nullopt;
}
