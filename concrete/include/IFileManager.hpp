#ifndef  ILRD_RD1645_IFILEMANAGER_HPP
#define  ILRD_RD1645_IFILEMANAGER_HPP

#include <cstddef>
#include <memory>

namespace ilrd
{
class IFileManager
{
public: 
    virtual bool ReadFromFile(size_t offset, size_t length, const std::shared_ptr<char[]>& buffer) = 0;
    virtual bool WriteToFile(size_t offset, size_t length, const std::shared_ptr<char[]>& buffer) = 0;
};
}

#endif