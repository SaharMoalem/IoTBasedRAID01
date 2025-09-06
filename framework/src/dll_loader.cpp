#include <dlfcn.h>          // dlopen, dlclose
#include <stdexcept>        // runtime_error    

#include "dll_loader.hpp"

using namespace ilrd;

DLLLoader::~DLLLoader()
{
    for(auto& handler : m_dll_map)
    {
        dlclose(handler.second);
    }
}

void DLLLoader::Load(std::string file_name)
{
    dl_handler handler = dlopen(file_name.c_str(), RTLD_LAZY);

    if(!handler)
    {
        throw std::runtime_error("Couldn't open plugin");
    }

    m_dll_map.emplace(file_name, handler);
}

void DLLLoader::Unload(std::string file_name)
{
    if(m_dll_map.find(file_name) == m_dll_map.end())
    {
        throw std::runtime_error("Plugin doesn't exist");
    }

    dlclose(m_dll_map[file_name]);
    m_dll_map.erase(file_name);
}