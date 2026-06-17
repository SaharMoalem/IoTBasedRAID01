#ifndef  __FACTORY_HPP__
#define  __FACTORY_HPP__

#include <functional>       // function
#include <unordered_map>    // unordered_map
#include <memory>           // unique_ptr
#include <mutex>

namespace ilrd
{

template<typename KEY, typename BASE, typename ...ARGS>
class Factory
{
public:
    Factory() = default;
    void Register(KEY key, std::function<std::shared_ptr<BASE>(ARGS...)> creator);
    std::shared_ptr<BASE> Create(KEY key, ARGS... args);

private:
    std::unordered_map<KEY, std::function<std::shared_ptr<BASE>(ARGS...)>> m_umap;
    mutable std::mutex m_mtx;
};

template<typename KEY, typename BASE, typename ...ARGS>
void Factory<KEY, BASE, ARGS...>::Register(KEY key, std::function<std::shared_ptr<BASE>(ARGS...)> creator)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_umap[key] = creator;
}

template<typename KEY, typename BASE, typename ...ARGS>
std::shared_ptr<BASE> Factory<KEY, BASE, ARGS...>::Create(KEY key, ARGS... args)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_umap.find(key);

    if(it == m_umap.end())
    {
        throw std::out_of_range("Key doesn't exist.");
    }

    return it->second(args...);
}
}


#endif