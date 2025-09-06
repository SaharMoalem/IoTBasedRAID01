#ifndef  ILRD_RD1645_ATASKARGS_HPP
#define  ILRD_RD1645_ATASKARGS_HPP

#include <cstddef>
#include <memory>

#include "ITaskArgs.hpp"
#include "UID.hpp"

namespace ilrd
{

struct TaskArgs
{
    TaskArgs(size_t offset, size_t length,
                            const std::shared_ptr<char[]>& buffer = nullptr);
    size_t m_offset;
    size_t m_length;
    std::shared_ptr<char[]> m_buffer;
};

class ATaskArgs : public ITaskArgs
{
public:
    ATaskArgs();
    ATaskArgs(size_t uid);
    UID GetUID() const;
    virtual int GetKey() const = 0;

private:
    UID m_uid;
};
}


#endif