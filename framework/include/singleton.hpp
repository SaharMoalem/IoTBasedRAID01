#ifndef  __SINGLETON_HPP__
#define  __SINGLETON_HPP__

#include <mutex>        // mutex
#include <atomic>

namespace ilrd
{
template<typename T>
class Singleton
{
public:
    Singleton() = delete;
    Singleton(const Singleton& other) = delete;
    Singleton& operator==(const Singleton& other) = delete;
    ~Singleton() = delete;
    static T* GetInstance();

private:
    static void DestroyInstance();
    static std::atomic<T*> m_singleton;
    static std::mutex m_mtx;
    
};

template<typename T>
std::atomic<T*> Singleton<T>::m_singleton = nullptr;

template <typename T>
std::mutex Singleton<T>::m_mtx;

template<typename T>
T* Singleton<T>::GetInstance()
{
    std::atomic_thread_fence(std::memory_order_acquire);

    if (m_singleton == nullptr)
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        std::atomic_thread_fence(std::memory_order_acquire);
        
        if (m_singleton == nullptr)
        {
            T* tmp = new T;
            m_singleton.store(tmp);
            std::atomic_thread_fence(std::memory_order_release);
            std::atexit(DestroyInstance);
        }
    }
    return m_singleton;
}

template<typename T>
void Singleton<T>::DestroyInstance()
{
    delete m_singleton;
    m_singleton.store(nullptr);
    std::atomic_thread_fence(std::memory_order_acquire);
}

}

#endif