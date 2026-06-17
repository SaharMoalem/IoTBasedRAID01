#include <cstring>

#include "MessageReceive.hpp"

using namespace ilrd;

MessageReadReceive::MessageReadReceive(size_t uid, bool result, size_t length,
    const std::shared_ptr<char[]> buffer):
    AMessage(uid), m_result(result), m_length(length),
    m_buffer(buffer)
{
    //std::copy(buffer, buffer + m_length + 1, m_buffer.get());
}

uint32_t MessageReadReceive::GetMessageType()
{
    return READ_RECEIVE;
}

uint32_t MessageReadReceive::GetSize() const
{
    return AMessage::GetSize() + sizeof(uint8_t) + sizeof(m_length) + m_length;
}

char* MessageReadReceive::ToBuffer(char* buffer)
{
    *(uint32_t*)(buffer) = GetSize();
    buffer += sizeof(uint32_t);
    *(uint32_t*)(buffer) = GetMessageType();
    buffer += sizeof(uint32_t);
    buffer = AMessage::ToBuffer(buffer);
    *(uint8_t*)(buffer) = static_cast<uint8_t>(m_result);
    buffer += sizeof(uint8_t);
    *(size_t*)(buffer) = m_length;
    buffer += sizeof(size_t);
    std::copy(m_buffer.get(), m_buffer.get() + m_length, buffer);

    return buffer + m_length;
}

char* MessageReadReceive::FromBuffer(char* buffer)
{
    buffer = AMessage::FromBuffer(buffer);
    m_result = *(uint8_t*)(buffer);
    buffer += sizeof(uint8_t);
    m_length = *(size_t*)(buffer);
    buffer += sizeof(size_t);
    m_buffer = std::make_shared<char[]>(m_length);
    std::copy(buffer, buffer + m_length, m_buffer.get());

    return buffer + m_length;
}

bool MessageReadReceive::GetResult() const
{
    return m_result != 0;
}

size_t MessageReadReceive::GetLength() const
{
    return m_length;
}

std::shared_ptr<char[]> MessageReadReceive::GetData() const
{
    return m_buffer;
}

MessageWriteReceive::MessageWriteReceive(size_t uid, bool result):
    AMessage(uid), m_result(result) {}

uint32_t MessageWriteReceive::GetMessageType()
{
    return WRITE_RECEIVE;
}

uint32_t MessageWriteReceive::GetSize() const
{
    return AMessage::GetSize() + sizeof(uint8_t);
}

char* MessageWriteReceive::ToBuffer(char* buffer)
{
    *(uint32_t*)(buffer) = GetSize();
    buffer += sizeof(uint32_t);
    *(uint32_t*)(buffer) = GetMessageType();
    buffer += sizeof(uint32_t);
    buffer = AMessage::ToBuffer(buffer);
    *(uint8_t*)(buffer) = static_cast<uint8_t>(m_result);

    return buffer + sizeof(uint8_t);
}

char* MessageWriteReceive::FromBuffer(char* buffer)
{
    buffer = AMessage::FromBuffer(buffer);
    m_result = *(uint8_t*)(buffer);

    return buffer + sizeof(uint8_t);
}

bool MessageWriteReceive::GetResult() const
{
    return m_result != 0;
}