#include "ATaskArgs.hpp"
// #include "handleton.hpp"
// #include "logger.hpp"

using namespace ilrd;

TaskArgs::TaskArgs(size_t offset, size_t length,
                                        const std::shared_ptr<char[]>& buffer):
    m_offset(offset),
    m_length(length),
    m_buffer(buffer) {}

ATaskArgs::ATaskArgs(): m_uid(UID()) {}

ATaskArgs::ATaskArgs(size_t uid): m_uid(uid) {}

UID ATaskArgs::GetUID() const
{
    return m_uid;
}