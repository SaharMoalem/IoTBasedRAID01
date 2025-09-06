#ifndef  ILRD_RD1645_NBDPROXY_HPP
#define  ILRD_RD1645_NBDPROXY_HPP

#include <map>
#include <linux/nbd.h>

#include "dispatcher.hpp"
#include "IInputProxy.hpp"
#include "ATaskArgs.hpp"
#include "Ticket.hpp"
#include "NBD.hpp"

namespace ilrd
{
class NBDProxy : public IInputProxy
{
public:
    explicit NBDProxy(NBD& nbd);
    ~NBDProxy() = default;

    std::shared_ptr<ITaskArgs> GetTaskArgs(int fd, FDMODE mode) override;
    void RegisterForNewTaskArgs(ACallback<std::shared_ptr<ATaskArgs>>& callback);
    void OnNBDResponse(const std::shared_ptr<TaskResult>& result);

private:
    Dispatcher<std::shared_ptr<ATaskArgs>> m_dispatcher;
    std::map<UID, nbd_reply> m_responses;
    std::mutex m_mtx;
    NBD& m_nbd;
};
}


#endif