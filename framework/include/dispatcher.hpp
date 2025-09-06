#ifndef  __DISPATCHER_HPP__
#define  __DISPATCHER_HPP__

#include <list>             // list
#include <functional>       // function

namespace ilrd
{
template<typename EVENT>
class ACallback;

/*********************************Dispatcher class*****************************/

template<typename EVENT>
class Dispatcher
{
public:
    ~Dispatcher();
    void Register(ACallback<EVENT>* callback);
    void Unregister(ACallback<EVENT>* callback);
    void Notify(EVENT event);

private:
    std::list<ACallback<EVENT>*> m_reg_list;
};

/**************************Callback Abstract class*****************************/

template<typename EVENT>
class ACallback
{
public:
    virtual ~ACallback();
    virtual void Notify(EVENT event) = 0;
    virtual void NotifyDeath() {}
    void SetDispatcher(Dispatcher<EVENT>* dispatcher);

private:
    Dispatcher<EVENT>* m_dispatcher;
};

/******************************Callback class**********************************/

template<typename EVENT, typename OBJECT>
class Callback: public ACallback<EVENT>
{
public:
    Callback(void(OBJECT::*notify)(EVENT), OBJECT& obj,
                                    void(OBJECT::*notify_death)() = nullptr);
    ~Callback() = default;
    void Notify(EVENT arg);
    void NotifyDeath();

private:
    void(OBJECT::*m_notify)(EVENT);
    OBJECT& m_obj;
    void(OBJECT::*m_notify_death)();
};

/*************************Dispatcher funcs definition**************************/
template<typename EVENT>
Dispatcher<EVENT>::~Dispatcher()
{
    std::list<ACallback<EVENT>*> list_copy(m_reg_list);

    for(ACallback<EVENT>* elem : list_copy)
    {
        elem->NotifyDeath();
        elem->SetDispatcher(nullptr);
    }
}

template<typename EVENT>
void Dispatcher<EVENT>::Register(ACallback<EVENT>* callback)
{
    callback->SetDispatcher(this);
    m_reg_list.push_back(callback);
}

template<typename EVENT>
void Dispatcher<EVENT>::Unregister(ACallback<EVENT>* callback)
{
    callback->SetDispatcher(nullptr);
    m_reg_list.remove(callback);
}

template<typename EVENT>
void Dispatcher<EVENT>::Notify(EVENT event)
{
    std::list<ACallback<EVENT>*> list_copy(m_reg_list);

    for(ACallback<EVENT>* elem : list_copy)
    {
        elem->Notify(event);
    }
}

/*************************ACallback funcs definition***************************/

template<typename EVENT>
ACallback<EVENT>::~ACallback()
{
    if(m_dispatcher)
    {
        m_dispatcher->Unregister(this);
    }
}

template<typename EVENT>
void ACallback<EVENT>::SetDispatcher(Dispatcher<EVENT>* dispatcher)
{
    m_dispatcher = dispatcher;
}

/*************************Callback funcs definition****************************/

template<typename EVENT, typename OBJECT>
Callback<EVENT, OBJECT>::Callback(void(OBJECT::*notify)(EVENT) , OBJECT& obj,
    void(OBJECT::*notify_death)()): m_notify(notify), m_obj(obj),
                                                m_notify_death(notify_death) {}

template<typename EVENT, typename OBJECT>
void Callback<EVENT, OBJECT>::Notify(EVENT arg)
{
    (m_obj.*m_notify)(arg);
}

template<typename EVENT, typename OBJECT>
void Callback<EVENT, OBJECT>::NotifyDeath()
{
    if(m_notify_death)
    {
        (m_obj.*m_notify_death)();
    }
}
}

#endif