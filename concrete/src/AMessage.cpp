#include "AMessage.hpp"

using namespace ilrd;

AMessage::AMessage(size_t uid): m_uid(uid){}

AMessage::~AMessage() {}

uint32_t AMessage::GetSize() const
{
    return sizeof(uint32_t) + sizeof(uint32_t) + sizeof(m_uid);
}

char* AMessage::ToBuffer(char* buffer)
{
    *(size_t*)buffer = m_uid;

    return buffer + sizeof(size_t);
}

char* AMessage::FromBuffer(char* buffer)
{
    m_uid = *(size_t*)buffer;

    return buffer + sizeof(size_t);
}

size_t AMessage::GetUID() const
{
    return m_uid;
}

size_t AMessage::GetOffset() const
{
    return 0;
}

size_t AMessage::GetLength() const
{
    return 0;
}

bool AMessage::GetResult() const
{
    return true;
}

std::shared_ptr<char[]> AMessage::GetData() const
{
    return nullptr;
}