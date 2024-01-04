#include "DeviceData.hpp"
#include "slic3r/GUI/GUI_App.hpp"

namespace Slic3r {

DeviceObject::DeviceObject(fnet_lan_dev_info *devInfo)
{
    m_devInfo = devInfo;
    m_dev_id = m_devInfo->serialNumber;
}

bool DeviceObject::is_lan_mode_printer()
{
    if(m_devInfo == nullptr)
        return false;

    if (m_devInfo->connectMode == 0)
        return true;
    else 
        return false;
}

bool DeviceObject::has_access_right()
{
    return !get_access_code().empty();
}

string DeviceObject::get_access_code() 
{
    return "";
}

void DeviceObject::set_user_access_code(const string &code, bool only_refresh /* = true*/)
{
    m_user_access_code = code;
    if (only_refresh && !code.empty()) {
        AppConfig *config = GUI::wxGetApp().app_config;
        if (config) {
            string dev_id;
            GUI::wxGetApp().app_config->set_str("user_access_code", dev_id, code);
        }
    }
}

string DeviceObject::get_user_access_code()
{
    AppConfig *config = GUI::wxGetApp().app_config;
    if (config) {
        string dev_id;
        return GUI::wxGetApp().app_config->get("user_access_code", dev_id);
    }
    return "";
}

void DeviceObject::erase_user_access_code()
{
    m_user_access_code = "";
    AppConfig *config      = GUI::wxGetApp().app_config;
    if (config) {
        string dev_id;
        GUI::wxGetApp().app_config->erase("user_access_code", dev_id);
        GUI::wxGetApp().app_config->save();
    }
}

string DeviceObject::get_dev_name()
{
    if (m_devInfo == nullptr)
        return "";
    return m_devInfo->name;
}

bool DeviceObject::is_in_printing_status(const string& status)
{
    if (status.compare("PAUSE") == 0 || status.compare("RUNNING") == 0 || status.compare("SLICING") == 0 || status.compare("PREPARE") == 0) {
        return true;
    }
    return false;
}

void DeviceObject::set_print_state(const string& status)
{
    m_printStatus = status;
}

bool DeviceObject::is_in_printing()
{
    /* use print_status if print_status is valid */
    if (!m_printStatus.empty())
        return DeviceObject::is_in_printing_status(m_printStatus);
    /*else {
        return DeviceObject::is_in_printing_status(iot_print_status);
    }*/
    return false;
}

}