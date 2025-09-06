#ifndef  __FW_TPTASK_HPP__
#define  __FW_TPTASK_HPP__

#include <memory>

#include "ITaskArgs.hpp"
#include "tp_task.hpp"

namespace ilrd
{
class FW_TPTask: public detail::ITPTask
{
public:
    explicit FW_TPTask(std::shared_ptr<ITaskArgs> task_args);
    ~FW_TPTask() = default;
    FW_TPTask(const FW_TPTask& other) = delete;
    FW_TPTask& operator=(const FW_TPTask& other) = delete;

    void Run();

private:
    std::shared_ptr<ITaskArgs> m_pTaskArg;
};
}

#endif