#ifndef  ILRD_RD1645_UID_HPP
#define  ILRD_RD1645_UID_HPP

#include <cstddef>

namespace ilrd
{

class UID
{
public:
    explicit UID();
    explicit UID(size_t uid);
    ~UID() = default;

    size_t GetID() const;
    bool operator<(const UID& other) const;

private:
    size_t m_id;
    static size_t s_counter;
};
}

#endif