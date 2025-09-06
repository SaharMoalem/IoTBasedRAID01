#ifndef  ILRD_RD1645_MINIONARGS_HPP
#define  ILRD_RD1645_MINIONARGS_HPP

#include "ATaskArgs.hpp"

namespace ilrd
{
class MinionReadArgs : public ATaskArgs
{
public:
    MinionReadArgs(size_t uid, const TaskArgs& args);
    int GetKey() const override;
    size_t GetOffset() const;
    size_t GetLength() const;

private:
    TaskArgs m_args;
};

class MinionWriteArgs : public ATaskArgs
{
public:
    MinionWriteArgs(size_t uid, const TaskArgs& args);
    int GetKey() const override;
    size_t GetOffset() const;
    size_t GetLength() const;
    std::shared_ptr<char[]> GetBuffer() const;

private:
    TaskArgs m_args;
};
}

#endif