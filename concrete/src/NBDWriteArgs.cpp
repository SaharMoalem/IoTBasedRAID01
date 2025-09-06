#include "NBDWriteArgs.hpp"
#include "IInputProxy.hpp"

using namespace ilrd;

NBDWriteArgs::NBDWriteArgs(const TaskArgs& args): ATaskArgs(), m_args(args) {}

int NBDWriteArgs::GetKey() const
{
    return FDMODE::WRITE;
}

size_t NBDWriteArgs::GetOffset() const
{
    return m_args.m_offset;
}

size_t NBDWriteArgs::GetLength() const
{
    return m_args.m_length;
}

std::shared_ptr<char[]> NBDWriteArgs::GetBuffer() const
{
    return m_args.m_buffer;
}