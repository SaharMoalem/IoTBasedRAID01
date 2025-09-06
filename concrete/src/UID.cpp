#include "UID.hpp"

using namespace ilrd;

size_t UID::s_counter = 0;

UID::UID(): m_id(s_counter++) {}

UID::UID(size_t uid): m_id(uid) {}

size_t UID::GetID() const
{
    return m_id;
}

bool UID::operator<(const UID& other) const
{
    return m_id < other.m_id;
}