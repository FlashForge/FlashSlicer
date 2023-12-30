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

class ComSendGcode : public ComCommand
{
public:
    ComSendGcode(const std::string &gcodeFileName, const std::string &thumbFileName,
        bool printNow, bool levelingBeforePrint, int commandId = -1)
        : ComCommand(commandId)
        , m_gcodeFileName(gcodeFileName)
        , m_thumbFileName(thumbFileName)
    {
        m_sendGcodeData.gcodeFileName = m_gcodeFileName.c_str();
        m_sendGcodeData.thumbFileName = m_thumbFileName.c_str();
        m_sendGcodeData.printNow = printNow;
        m_sendGcodeData.levelingBeforePrint = levelingBeforePrint;
        m_sendGcodeData.callback = nullptr;
        m_sendGcodeData.callbackData = nullptr;
    }
    ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &ip,
        unsigned int port, const std::string &serialNumber, const std::string &checkCode)
    {
        int ret = networkIntfc->lanDevSendGcode(
            ip.c_str(), port, serialNumber.c_str(), checkCode.c_str(), &m_sendGcodeData);
        return MultiComUtils::networkIntfcRet2ComErrno(ret);
    }
    ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &accessToken,
        const std::string &deviceId)
    {
        return COM_ERROR;
    }
    void setCallback(fnet_progress_call_back_t callback, void *callbackData)
    {
        m_sendGcodeData.callback = callback;
        m_sendGcodeData.callbackData = callbackData;
    }

private:
    std::string m_gcodeFileName;
    std::string m_thumbFileName;
    fnet_send_gcode_data_t m_sendGcodeData;
};

}} // namespace Slic3r::GUI

#endif
