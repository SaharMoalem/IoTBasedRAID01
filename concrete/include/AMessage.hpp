#ifndef  ILRD_RD1645_AMESSAGE_HPP
#define  ILRD_RD1645_AMESSAGE_HPP

#include <memory>
#include "UID.hpp"

namespace ilrd
{

enum MESSAGE_TYPE
{
    READ_SEND,
    WRITE_SEND,
    READ_RECEIVE,
    WRITE_RECEIVE
};

class AMessage
{
public:
    explicit AMessage(size_t uid = 0);
    virtual ~AMessage() = 0;

    virtual uint32_t GetMessageType() = 0;
    virtual char* ToBuffer(char* buffer);
    virtual char* FromBuffer(char* buffer);
    virtual uint32_t GetSize() const;
    virtual size_t GetOffset() const;
    virtual size_t GetLength() const;
    virtual bool GetResult() const;
    virtual std::shared_ptr<char[]> GetData() const;

    size_t GetUID() const;

private:
    size_t m_uid;
};
}

#endif