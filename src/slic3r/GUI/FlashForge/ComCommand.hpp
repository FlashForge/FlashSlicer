#ifndef slic3r_GUI_ComCommand_hpp_
#define slic3r_GUI_ComCommand_hpp_

#include "FlashNetworkIntfc.h"
#include "MultiComDef.hpp"
#include "MultiComUtils.hpp"

namespace Slic3r { namespace GUI {

class ComCommand
{
public:
    ComCommand(int commandId = -1)
        : m_commandId(commandId)
    {
    }
    virtual ~ComCommand()
    {
    }
    int commandId() const
    {
        return m_commandId;
    }
    virtual bool isDup(const ComCommand *that)
    {
        return typeid(*this) == typeid(*that);
    }
    virtual ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &ip,
        unsigned int port, const std::string &serialNumber, const std::string &checkCode) = 0;

    virtual ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &accessToken,
        const std::string &deviceId) = 0;

private:
    int m_commandId;
};

class ComGetDevDetail : public ComCommand
{
public:
    ComGetDevDetail(int commandId = -1)
        : ComCommand(commandId), m_devDetail(nullptr)
    {
    }
    ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &ip,
        unsigned int port, const std::string &serialNumber, const std::string &checkCode)
    {
        int ret = networkIntfc->getLanDevDetail(
            ip.c_str(), port, serialNumber.c_str(), checkCode.c_str(), &m_devDetail);
        return MultiComUtils::networkIntfcRet2ComErrno(ret);
    }
    ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &accessToken,
        const std::string &deviceId)
    {
        int ret = networkIntfc->getWanDevDetail(
            accessToken.c_str(), deviceId.c_str(), &m_devDetail);
        return MultiComUtils::networkIntfcRet2ComErrno(ret);
    }
    fnet_dev_detail_t *devDetail()
    {
        return m_devDetail;
    }
 
private:
    fnet_dev_detail_t *m_devDetail;
};

}} // namespace Slic3r::GUI

#endif
