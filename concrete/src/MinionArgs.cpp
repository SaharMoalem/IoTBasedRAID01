#include "MinionArgs.hpp"
#include "IInputProxy.hpp"

using namespace ilrd;

MinionReadArgs::MinionReadArgs(size_t uid, const TaskArgs& args):
    ATaskArgs(uid), m_args(args) {}

int MinionReadArgs::GetKey() const
{
    return FDMODE::READ;
}

size_t MinionReadArgs::GetOffset() const
{
    return m_args.m_offset;
}

size_t MinionReadArgs::GetLength() const
{
    return m_args.m_length;
}

MinionWriteArgs::MinionWriteArgs(size_t uid, const TaskArgs& args):
    ATaskArgs(uid), m_args(args) {}

int MinionWriteArgs::GetKey() const
{
    return FDMODE::WRITE;
}

size_t MinionWriteArgs::GetOffset() const
{
    return m_args.m_offset;
}

size_t MinionWriteArgs::GetLength() const
{
    return m_args.m_length;
}

std::shared_ptr<char[]> MinionWriteArgs::GetBuffer() const
{
    return m_args.m_buffer;
}