#ifndef  __HANDLETON_HPP__
#define  __HANDLETON_HPP__

#include <memory>           // shared_ptr
#include <unordered_map>    // unordered_map
#include <mutex>  

namespace ilrd
{
class Handleton
{
public:
    template<typename T>
    static T* GetInstance();

private:
    class HashFunction
    {
    public:
        size_t operator()(const std::type_info* id) const
        {
            return id->hash_code();
        }
    };

    class Comparator
    {
    public:
        bool operator()(const std::type_info* lhs, const std::type_info* rhs) const
        {
            return *lhs == *rhs;
        }
    };

    static std::unordered_map<const std::type_info*, std::shared_ptr<void>,
                                        HashFunction, Comparator> m_handletons;
    static std::mutex m_mtx;
    
    explicit Handleton() = delete;
    ~Handleton() = delete;
    Handleton(const Handleton& other) = delete;
    Handleton& operator=(const Handleton& other) = delete;
};

template<typename T>
T* Handleton::GetInstance()
{
    static std::once_flag flag;
    std::call_once(flag, []
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        if (!m_handletons.contains(&typeid(T)))
        {
            m_handletons[&typeid(T)].reset(new T);
        }
    });

    return static_cast<T*>(m_handletons.at(&typeid(T)).get());
}
}

#endif