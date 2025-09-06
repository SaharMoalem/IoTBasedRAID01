#ifndef  ILRD_RD1645_WRITEARGS_HPP
#define  ILRD_RD1645_WRITEARGS_HPP

#include "ATaskArgs.hpp"

namespace ilrd
{
class NBDWriteArgs : public ATaskArgs
{
public:
    NBDWriteArgs(const TaskArgs& args);
    int GetKey() const override;
    size_t GetOffset() const;
    size_t GetLength() const;
    std::shared_ptr<char[]> GetBuffer() const;

private:
    TaskArgs m_args;
};
}

#endif