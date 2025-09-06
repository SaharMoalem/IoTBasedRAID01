#ifndef  ILRD_RD1645_FILEMANAGER_HPP
#define  ILRD_RD1645_FILEMANAGER_HPP

#include <fstream>

#include "IFileManager.hpp"
#include "handleton.hpp"

namespace ilrd
{
class FileManager: public IFileManager
{
public:
    ~FileManager() = default;
    
    void Init(const std::string& file);
    bool ReadFromFile(size_t offset, size_t length, const std::shared_ptr<char[]>& buffer) override;
    bool WriteToFile(size_t offset, size_t length, const std::shared_ptr<char[]>& buffer) override;

private:
    friend class Handleton;
    FileManager() = default;

    std::fstream m_stream;
    std::mutex m_mtx;
};
}

#endif