#ifndef  ILRD_RD1645_READARGS_HPP
#define  ILRD_RD1645_READARGS_HPP

#include "ATaskArgs.hpp"

namespace ilrd
{
class NBDReadArgs : public ATaskArgs
{
public:
    NBDReadArgs(const TaskArgs& args);
    int GetKey() const override;
    size_t GetOffset() const;
    size_t GetLength() const;

private:
    TaskArgs m_args;
};
}

#endif