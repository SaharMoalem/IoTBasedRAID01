#include <cstring>
#include <iostream>

#include "MessageSend.hpp"
#include "MessageReceive.hpp"

using namespace ilrd;

int main()
{
    // MessageReadSend rsMsg(1, 2, 3);
    // MessageReadSend rsMsg2(4, 5, 6);
    // char* buffer = new char[rsMsg.GetSize()];
    // rsMsg.ToBuffer(buffer);
    // char* helper = buffer;
    // uint32_t size = *(uint32_t*)buffer;
    // buffer += sizeof(uint32_t) + sizeof(uint32_t);
    // std::cout << size;
    // rsMsg2.FromBuffer(buffer);
    // rsMsg2.ToString();

    // delete[] helper;
    // MessageWriteSend wsMsg(1, 2, 6, "Hello");
    // MessageWriteSend wsMsg2(1, 2, 6, "hELLO");
    MessageReadSend rMsg(0, 1, 2);
    char* buffer = new char[rMsg.GetSize()];
    rMsg.ToBuffer(buffer);
    
    MessageReadSend tMsg;
    char* helper = buffer;
    uint32_t size = *(uint32_t*)buffer;
    buffer += sizeof(uint32_t);
    uint32_t type = *(uint32_t*)buffer;
    buffer += sizeof(uint32_t);
    tMsg.FromBuffer(buffer);

    std::cout << size << " " << type << " " << tMsg.GetUID()
        << " " << tMsg.GetOffset() << " " << tMsg.GetLength() << std::endl;

    delete[] helper;

    return 0;
}