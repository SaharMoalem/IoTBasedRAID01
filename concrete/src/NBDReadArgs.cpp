#include "NBDReadArgs.hpp"
#include "IInputProxy.hpp"

using namespace ilrd;

NBDReadArgs::NBDReadArgs(const TaskArgs& args): ATaskArgs(), m_args(args) {}

int NBDReadArgs::GetKey() const
{
    return FDMODE::READ;
}

size_t NBDReadArgs::GetOffset() const
{
    return m_args.m_offset;
}

size_t NBDReadArgs::GetLength() const
{
    return m_args.m_length;
}