#ifndef slic3r_GUI_ComCommand_hpp_
#define slic3r_GUI_ComCommand_hpp_

#include <wx/event.h>
#include "FlashNetworkIntfc.h"
#include "MultiComDef.hpp"
#include "MultiComEvent.hpp"
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

protected:
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
        return MultiComUtils::fnetRet2ComErrno(ret);
    }
    ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &accessToken,
        const std::string &deviceId)
    {
        int ret = networkIntfc->getWanDevDetail(accessToken.c_str(), deviceId.c_str(), &m_devDetail);
        return MultiComUtils::fnetRet2ComErrno(ret);
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
        , m_progress(0)
        , m_comId(ComInvalidId)
        , m_evtHandler(nullptr)
        , m_gcodeFileName(gcodeFileName)
        , m_thumbFileName(thumbFileName)
    {
        m_sendGcodeData.gcodeFileName = m_gcodeFileName.c_str();
        m_sendGcodeData.thumbFileName = m_thumbFileName.c_str();
        m_sendGcodeData.printNow = printNow;
        m_sendGcodeData.levelingBeforePrint = levelingBeforePrint;
        m_sendGcodeData.callback = callback;
        m_sendGcodeData.callbackData = this;
    }
    ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &ip,
        unsigned int port, const std::string &serialNumber, const std::string &checkCode)
    {
        int ret = networkIntfc->lanDevSendGcode(
            ip.c_str(), port, serialNumber.c_str(), checkCode.c_str(), &m_sendGcodeData);
        return MultiComUtils::fnetRet2ComErrno(ret);
    }
    ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc, const std::string &accessToken,
        const std::string &deviceId)
    {
        int ret = networkIntfc->wanDevSendGcode(
            accessToken.c_str(), deviceId.c_str(), &m_sendGcodeData);
        return MultiComUtils::fnetRet2ComErrno(ret);
    }
    void setConectionData(com_id_t comId, wxEvtHandler *evtHandler)
    {
        m_comId = comId;
        m_evtHandler = evtHandler;
    }

private:
    static void callback(long long now, long long total, void *callbackData)
    {
        double progress = (double)now / total;
        ComSendGcode *inst = (ComSendGcode *) callbackData;
        if (progress - inst->m_progress > 0.025 && inst->m_evtHandler != nullptr) {
            inst->m_evtHandler->QueueEvent(new ComSendGcodeProgressEvent(
                COM_SEND_GCODE_PROGRESS_EVENT, inst->m_comId, inst->m_commandId, now, total));
            inst->m_progress = progress;
        }
    }

private:
    double m_progress;
    com_id_t m_comId;
    wxEvtHandler *m_evtHandler;
    std::string m_gcodeFileName;
    std::string m_thumbFileName;
    fnet_send_gcode_data_t m_sendGcodeData;
};

}} // namespace Slic3r::GUI

#endif
