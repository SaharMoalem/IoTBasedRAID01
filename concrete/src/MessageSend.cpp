#include <cstring>
#include <iostream>

#include "MessageSend.hpp"

using namespace ilrd;

MessageReadSend::MessageReadSend(size_t uid, size_t offset, size_t length):
                            AMessage(uid), m_offset(offset), m_length(length) {}

uint32_t MessageReadSend::GetMessageType()
{
    return READ_SEND;
}

uint32_t MessageReadSend::GetSize() const
{
    return AMessage::GetSize() + sizeof(m_offset) + sizeof(m_length);
}

char* MessageReadSend::ToBuffer(char* buffer)
{
    *(uint32_t*)(buffer) = GetSize();
    buffer += sizeof(uint32_t);
    *(uint32_t*)(buffer) = GetMessageType();
    buffer += sizeof(uint32_t);
    buffer = AMessage::ToBuffer(buffer);
    *(size_t*)(buffer) = m_offset;
    buffer += sizeof(size_t);
    *(size_t*)(buffer) = m_length;

    return buffer + sizeof(size_t);
}

char* MessageReadSend::FromBuffer(char* buffer)
{
    buffer = AMessage::FromBuffer(buffer);
    m_offset = *(size_t*)buffer;
    buffer += sizeof(size_t);
    m_length = *(size_t*)buffer;

    return buffer + sizeof(size_t);
}

size_t MessageReadSend::GetOffset() const
{
    return m_offset;
}

size_t MessageReadSend::GetLength() const
{
    return m_length;
}

void MessageReadSend::ToString()
{
    std::cout << GetUID() << " " << m_offset << " " << m_length << std::endl;
}

MessageWriteSend::MessageWriteSend(size_t uid, size_t offset, size_t length,
    const std::shared_ptr<char[]> buffer):
    AMessage(uid), m_offset(offset), m_length(length), m_buffer(buffer)
{}

uint32_t MessageWriteSend::GetMessageType()
{
    return WRITE_SEND;
}

char* MessageWriteSend::ToBuffer(char* buffer)
{
    *(uint32_t*)(buffer) = GetSize();
    buffer += sizeof(uint32_t);
    *(uint32_t*)(buffer) = GetMessageType();
    buffer += sizeof(uint32_t);
    buffer = AMessage::ToBuffer(buffer);
    *(size_t*)(buffer) = m_offset;
    buffer += sizeof(size_t);
    *(size_t*)(buffer) = m_length;
    buffer += sizeof(size_t);
    std::copy(m_buffer.get(), m_buffer.get() + m_length + 1, buffer);

    return buffer + m_length + 1;
}

char* MessageWriteSend::FromBuffer(char* buffer)
{
    buffer = AMessage::FromBuffer(buffer);
    m_offset = *(size_t*)buffer;
    buffer += sizeof(size_t);
    m_length = *(size_t*)buffer;
    buffer += sizeof(size_t);
    m_buffer = std::make_shared<char[]>(m_length + 1);
    std::copy(buffer, buffer + m_length + 1, m_buffer.get());

    return buffer + m_length + 1;
}

uint32_t MessageWriteSend::GetSize() const
{
    return AMessage::GetSize() + sizeof(m_offset) + sizeof(m_length) + m_length
                                                                            + 1;
}

size_t MessageWriteSend::GetOffset() const
{
    return m_offset;
}

size_t MessageWriteSend::GetLength() const
{
    return m_length;
}

std::shared_ptr<char[]> MessageWriteSend::GetData() const
{
    return m_buffer;
}

void MessageWriteSend::ToString()
{
    std::cout << GetUID() << " " << m_offset << " " << m_length << " " << m_buffer << std::endl;
}