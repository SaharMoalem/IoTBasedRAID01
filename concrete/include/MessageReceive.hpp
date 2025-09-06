#ifndef  ILRD_RD1645_MESSAGERECEIVE_HPP
#define  ILRD_RD1645_MESSAGERECEIVE_HPP

#include "AMessage.hpp"

namespace ilrd
{

class MessageReadReceive: public AMessage
{
public:
    MessageReadReceive() = default;
    MessageReadReceive(size_t uid, bool result, size_t length, const std::shared_ptr<char[]> buffer);
    ~MessageReadReceive() = default;

    uint32_t GetMessageType() override;
    char* ToBuffer(char* buffer) override;
    char* FromBuffer(char* buffer) override;
    uint32_t GetSize() const override;
    bool GetResult() const override;
    size_t GetLength() const override;
    std::shared_ptr<char[]> GetData() const override;

private:
    bool m_result;
    size_t m_length;
    std::shared_ptr<char[]> m_buffer;
};

class MessageWriteReceive: public AMessage
{
public:
    MessageWriteReceive() = default;
    MessageWriteReceive(size_t uid, bool result);
    ~MessageWriteReceive() = default;

    uint32_t GetMessageType() override;
    char* ToBuffer(char* buffer) override;
    char* FromBuffer(char* buffer) override;
    uint32_t GetSize() const override;
    bool GetResult() const override;

private:
    bool m_result;
};
}

#endif