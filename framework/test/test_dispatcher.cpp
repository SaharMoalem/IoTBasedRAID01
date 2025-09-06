#include <iostream>
#include "dispatcher.hpp"

class Moshe
{
public:
    Moshe() = default;
    void Do(int i) {std::cout << "Moshe::Do " << i << std::endl; }
    void DoHast() {std::cout << "Moshe::DoHast" << std::endl; }
    void DoHastMich(int i) {std::cout << "Moshe::DoHastMich " << i << std::endl; }
};

void TestDispatcher()
{
    ilrd::Dispatcher<int> myDispatcher;
    Moshe moshe;
    ilrd::Callback<int, Moshe> cb1((&Moshe::Do), moshe, (&Moshe::DoHast));
    ilrd::Callback<int, Moshe> cb2((&Moshe::DoHastMich), moshe, (&Moshe::DoHast));
    myDispatcher.Register(&cb1);
    myDispatcher.Register(&cb2);
    myDispatcher.Notify(5);
    myDispatcher.Unregister(&cb1);
    myDispatcher.Notify(10);
    myDispatcher.Unregister(&cb2);
    myDispatcher.Notify(7);
}

int main()
{
    TestDispatcher();
    return 0;
}

// #include <iostream>

// #include "dispatcher.hpp"

// class producer
// {
// public:
//     struct full
//     {
//         int a;
//     };
//     ilrd::Dispatcher<full> im_noti;
//     void Noti();

// };

// void producer::Noti()
// {
//     full data = {4};
//     im_noti.Notify(data);
// }

// class consumer1
// {
// public:
//     void urnoti(producer::full data_)
//     {
//         std::cout << "first Callback " << data_.a << std::endl;
//     }
// };

// class consumer2
// {
// public:
//     void urnoti(producer::full data_)
//     {
//         std::cout << "second Callback " << data_.a << std::endl;
//     }
// };

// class consumer3
// {
// public:
//     void urnoti(producer::full data_)
//     {
//         std::cout << "third Callback " << data_.a << std::endl;
//     }
//     void unoti()
//     {
//         std::cout << "death of third Callback " << std::endl;
//     }
// };


// void DispatcherTest()
// {
//     consumer1 cons1;
//     consumer2 cons2;
//     consumer3 cons3;
//     producer ofir;

//     ilrd::Callback<producer::full, consumer1> runoti1(cons1, &consumer1::urnoti);
//     ilrd::Callback<producer::full, consumer2> runoti2(cons2, &consumer2::urnoti);
//     ilrd::Callback<producer::full, consumer3> runoti3(cons3,
//     &consumer3::urnoti, &consumer3::unoti);

//     ofir.im_noti.Register(&runoti1);
//     ofir.im_noti.Register(&runoti1);
//     ofir.im_noti.Register(&runoti2);
//     ofir.im_noti.Register(&runoti3);

//     ofir.Noti();

//     ofir.im_noti.Unregister(&runoti1);
//     ofir.im_noti.Unregister(&runoti1);

//     ofir.Noti();

// }



// int main()
// {
//     DispatcherTest();
//     return 0;
// }