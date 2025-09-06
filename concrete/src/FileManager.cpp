#include "FileManager.hpp"
#include "MasterProxy.hpp"

using namespace ilrd;

void FileManager::Init(const std::string& file)
{
    m_stream.open(file, std::ios::out | std::ios::in);
}

bool FileManager::ReadFromFile(size_t offset, size_t length,
                                        const std::shared_ptr<char[]>& buffer)
{
    std::unique_lock lock(m_mtx);
    m_stream.seekg(offset);
    m_stream.read(buffer.get(), length);

    return m_stream.good();
}

bool FileManager::WriteToFile(size_t offset, size_t length,
                                        const std::shared_ptr<char[]>& buffer)
{
    std::unique_lock lock(m_mtx);
    m_stream.seekp(offset);
    m_stream.write(buffer.get(), length);
    m_stream.flush();

    return m_stream.good();
}