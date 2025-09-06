#ifndef  ILRD_RD1645_MESSAGESEND_HPP
#define  ILRD_RD1645_MESSAGESEND_HPP

#include "AMessage.hpp"

namespace ilrd
{
class MessageReadSend: public AMessage
{
public:
    MessageReadSend() = default;
    MessageReadSend(size_t uid, size_t offset, size_t length);
    ~MessageReadSend() = default;

    uint32_t GetMessageType() override;
    char* ToBuffer(char* buffer) override;
    char* FromBuffer(char* buffer) override;
    uint32_t GetSize() const override;
    size_t GetOffset() const  override;
    size_t GetLength() const override;
    void ToString(); //test purpose

private:
    size_t m_offset;
    size_t m_length;
};

class MessageWriteSend: public AMessage
{
public:
    MessageWriteSend() = default;
    MessageWriteSend(size_t uid, size_t offset, size_t length,
                                        const std::shared_ptr<char[]> buffer);
    ~MessageWriteSend() = default;

    uint32_t GetMessageType() override;
    char* ToBuffer(char* buffer) override;
    char* FromBuffer(char* buffer) override;
    uint32_t GetSize() const override;
    size_t GetOffset() const override;
    size_t GetLength() const override;
    std::shared_ptr<char[]> GetData() const override;
    void ToString(); //test purpose

private:
    size_t m_offset;
    size_t m_length;
    std::shared_ptr<char[]> m_buffer;
};
}

#endif