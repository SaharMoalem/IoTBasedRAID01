#ifndef  ILRD_RD1645_RESPONSE_HPP
#define  ILRD_RD1645_RESPONSE_HPP

#include "IResponse.hpp"

namespace ilrd
{
class Response: public IResponse
{
public:
    Response() = default;
    ~Response() = default;

    void OnSuccess();
    void OnFailure();
};
}

#endif