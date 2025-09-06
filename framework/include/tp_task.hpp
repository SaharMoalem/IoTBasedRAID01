#ifndef  __TP_TASK_HPP__
#define  __TP_TASK_HPP__

#include <functional>   // function
#include <semaphore>    // binary, counting semaphore
#include <tuple>        // tuple

namespace ilrd::detail
{

/*****************************Task Interface***********************************/

class ITPTask
{
public:
    virtual void Run() = 0;
    virtual ~ITPTask() = default;
};

/*****************************Void Task****************************************/

template<typename ...ARGS>
class FunctionTask: public ITPTask
{
public:
    FunctionTask(std::function<void(ARGS...)> func, ARGS... args);
    ~FunctionTask() = default;
    void Run();

private:
    std::function<void(ARGS...)> m_func;
    std::tuple<ARGS...> m_args;
};


/*****************************RETURN Task**************************************/

template<typename RETURN, typename ...ARGS>
class FutureFunction: public ITPTask
{
public:
    FutureFunction(std::function<RETURN(ARGS...)> func, RETURN& target, ARGS... args);
    ~FutureFunction() = default;
    void Run();
    RETURN& Get() const;

private:
    std::function<RETURN(ARGS...)> m_func;
    std::tuple<ARGS...> m_args;
    RETURN& m_return_value;
    mutable std::binary_semaphore m_sem;
};

/************************Functions definitions*********************************/
template <typename ...ARGS>
FunctionTask<ARGS...>::FunctionTask(std::function<void(ARGS...)> func, ARGS... args):
    m_func(func), m_args(std::make_tuple(std::ref(args)...)) {}

template <typename ...ARGS>
void FunctionTask<ARGS...>::Run()
{
    std::apply(m_func, m_args);
}

template<typename RETURN, typename ...ARGS>
FutureFunction<RETURN, ARGS...>::FutureFunction(std::function<RETURN(ARGS...)> func, RETURN& target, ARGS... args):
    m_func(func), m_args(std::make_tuple(args...)), m_return_value(target), m_sem(0) {}

template<typename RETURN, typename ...ARGS>
void FutureFunction<RETURN, ARGS...>::Run()
{
    m_return_value = std::apply(m_func, m_args);
    m_sem.release();
}

template<typename RETURN, typename ...ARGS>
RETURN& FutureFunction<RETURN, ARGS...>::Get() const
{
    m_sem.acquire();
    return m_return_value;
}

}

#endif