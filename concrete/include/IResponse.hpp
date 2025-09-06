#ifndef  ILRD_RD1645_IRESPONSE_HPP
#define  ILRD_RD1645_IRESPONSE_HPP

namespace ilrd
{
class IResponse
{
public:
    virtual void OnSuccess() = 0;
    virtual void OnFailure() = 0;
};
}

#endif