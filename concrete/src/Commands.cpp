#include "Commands.hpp"
#include "handleton.hpp"
#include "MinionManager.hpp"
#include "NBDReadArgs.hpp"
#include "NBDWriteArgs.hpp"

using namespace ilrd;

std::optional<std::pair<AsyncFunc, std::chrono::milliseconds>> ReadCommand::Run(std::shared_ptr<ITaskArgs> args)
{
    try
    {
        NBDReadArgs* readArgs = dynamic_cast<NBDReadArgs*>(args.get());
        Handleton::GetInstance<MinionManager>()->AddReadTask(readArgs->GetUID(), readArgs->GetOffset(),
                                                        readArgs->GetLength());
    }
    catch(...)
    {

    }

    // return async
    return std::make_pair([](){ return true;/*?*/ }, std::chrono::milliseconds(100));
}

std::optional<std::pair<AsyncFunc, std::chrono::milliseconds>> WriteCommand::Run(std::shared_ptr<ITaskArgs> args)
{
    try
    {
        NBDWriteArgs* writeArgs = dynamic_cast<NBDWriteArgs*>(args.get());
        Handleton::GetInstance<MinionManager>()->AddWriteTask(writeArgs->GetUID(),
            writeArgs->GetOffset(), writeArgs->GetLength(),
                                                        writeArgs->GetBuffer());
    }
    catch(...)
    {

    }

    // return async
    return std::make_pair([](){ return true;/*?*/ }, std::chrono::milliseconds(100));
}