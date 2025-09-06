#include "MinionCommands.hpp"
#include "MinionArgs.hpp"
#include "handleton.hpp"
#include "FileManager.hpp"
#include "MasterProxy.hpp"

using namespace ilrd;

std::optional<std::pair<ilrd::AsyncFunc, std::chrono::milliseconds>> MinionReadCommand::Run(std::shared_ptr<ilrd::ITaskArgs> args)
{
    try
    {
        MinionReadArgs* readArgs = dynamic_cast<MinionReadArgs*>(args.get());
        std::shared_ptr<char[]> readBuffer(new char[readArgs->GetLength()]);
        bool result = Handleton::GetInstance<FileManager>()->ReadFromFile(readArgs->GetOffset(),
                                            readArgs->GetLength(), readBuffer);
        Handleton::GetInstance<MasterProxy>()->SendReadResponse(readArgs->GetUID(),
                                    result, readArgs->GetLength(), readBuffer);
    }
    catch(...)
    {

    }
    
    return std::make_pair([](){ return true;/*?*/ }, std::chrono::milliseconds(100));
}

std::optional<std::pair<ilrd::AsyncFunc, std::chrono::milliseconds>> MinionWriteCommand::Run(std::shared_ptr<ilrd::ITaskArgs> args)
{
    try
    {
        MinionWriteArgs* writeArgs = dynamic_cast<MinionWriteArgs*>(args.get());
        bool result = Handleton::GetInstance<FileManager>()->WriteToFile(writeArgs->GetOffset(),
                                writeArgs->GetLength(), writeArgs->GetBuffer());
        Handleton::GetInstance<MasterProxy>()->SendWriteResponse(writeArgs->GetUID(),
                                                                        result);
    }
    catch(...)
    {

    }
    
    return std::make_pair([](){ return true;/*?*/ }, std::chrono::milliseconds(100));
}