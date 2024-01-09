#include "DeviceData.hpp"
#include "slic3r/GUI/GUI_App.hpp"
#include "MultiComMgr.hpp"

namespace Slic3r {

namespace GUI {

DeviceObject::DeviceObject(const fnet_lan_dev_info &devInfo)
    :m_is_online(false)
{
    m_devInfo = new fnet_lan_dev_info(devInfo);
    m_dev_id = m_devInfo->serialNumber;
    m_bind_state = "free";
}

bool DeviceObject::is_lan_mode_in_scan_print()
{
    if(m_devInfo != nullptr && m_devInfo->connectMode == 0)
        return true;
    return false;
}

bool DeviceObject::is_lan_mode_printer()
{
    if (!m_dev_connection_type.empty() && m_dev_connection_type == CONNECTTYPE_LAN)
        return true;
    return false;
}

bool DeviceObject::has_access_right()
{
    return !get_access_code().empty();
}

void DeviceObject::set_access_code(const string &code, bool only_refresh /* = true*/)
{
    m_access_code = code;
    if (only_refresh) {
        AppConfig *config = GUI::wxGetApp().app_config;
        if (config && !code.empty()) {
            GUI::wxGetApp().app_config->set_str("access_code", m_dev_id, code);
        }
    }
}

string DeviceObject::get_access_code() 
{
    if (get_user_access_code().empty())
        return m_access_code;
    return get_user_access_code();
}

void DeviceObject::set_user_access_code(const string &code, bool only_refresh /* = true*/)
{
    m_user_access_code = code;
    if (only_refresh && !code.empty()) {
        AppConfig *config = GUI::wxGetApp().app_config;
        if (config) {
            GUI::wxGetApp().app_config->set_str("user_access_code", m_dev_id, code);
            GUI::wxGetApp().app_config->set_str("user_dev_name", m_dev_id, get_dev_name());
        }
    }
}

string DeviceObject::get_user_access_code()
{
    AppConfig *config = GUI::wxGetApp().app_config;
    if (config) {
        return GUI::wxGetApp().app_config->get("user_access_code", m_dev_id);
    }
    return "";
}

void DeviceObject::erase_user_access_code()
{
    m_user_access_code = "";
    AppConfig *config      = GUI::wxGetApp().app_config;
    if (config) {
        GUI::wxGetApp().app_config->erase("user_access_code", m_dev_id);
        GUI::wxGetApp().app_config->erase("user_dev_name", m_dev_id);
        GUI::wxGetApp().app_config->save();
    }
}

bool DeviceObject::is_avaliable()
{
    return m_bind_state == "free";
}

void DeviceObject::set_online_state(bool on_off)
{
    m_is_online = on_off;
    if (!on_off)
        m_active_state = NotActive;
}

bool DeviceObject::is_online()
{
    return m_is_online;
}

void DeviceObject::set_active_state(ActiveState state)
{
    m_active_state = state;
}

ActiveState DeviceObject::get_active_state()
{
    return m_active_state;
}

void DeviceObject::set_connection_type(const string& connectType)
{
    m_dev_connection_type = connectType;
}

string DeviceObject::connection_type() 
{
    return m_dev_connection_type; 
}

void DeviceObject::reset_update_time()
{
    BOOST_LOG_TRIVIAL(trace) << "reset reset_update_time, dev_id =" << m_dev_id;
    last_update_time = std::chrono::system_clock::now();
}

fnet_lan_dev_info * DeviceObject::get_lan_dev_info()
{
    return m_devInfo;
}

string DeviceObject::get_dev_name()
{
    if (m_devInfo == nullptr)
        return "";
    return m_devInfo->name;
}

string DeviceObject::get_dev_id()
{
    return m_dev_id;
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






DeviceObjectOpr::DeviceObjectOpr()
{
    MultiComMgr::inst()->Bind(COM_CONNECTION_EXIT_EVENT, &DeviceObjectOpr::onEraseAccessCode, this);
    MultiComMgr::inst()->Bind(COM_CONNECTION_READY_EVENT, &DeviceObjectOpr::onUpdateUserMachine, this);
}

DeviceObjectOpr::~DeviceObjectOpr()
{
    for (auto it = m_scan_devices.begin(); it != m_scan_devices.end(); it++) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_scan_devices.clear();

    for (auto it = m_user_devices.begin(); it != m_user_devices.end(); it++) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_user_devices.clear();

    for (auto it = m_local_devices.begin(); it != m_local_devices.end(); it++) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_local_devices.clear();
}

void DeviceObjectOpr::update_scan_machine()
{
    std::vector<fnet_lan_dev_info> devInfos;
    MultiComUtils::getLanDevList(devInfos);

    for (auto &elem : devInfos) {
        string dev_id = elem.serialNumber;
        if (m_scan_devices.find(dev_id) == m_scan_devices.end()) {
            DeviceObject *devObj = new DeviceObject(elem);
            devObj->set_connection_type(CONNECTTYPE_LAN);
            m_scan_devices.insert(make_pair(dev_id, devObj));
        }
    }

    auto it = m_scan_devices.begin();
    for (it; it != m_scan_devices.end();) {
        string dev_id = it->first;
        bool bFind = false;
        for (int i = 0; i < devInfos.size(); ++i) {
            if (devInfos[i].serialNumber == dev_id) {
                bFind = true;
                break;
            }
        }
        if (!bFind) {
            delete it->second;
            it = m_scan_devices.erase(it);
        } else {
            ++it;
        }
    }
}

// connect_type: cloud
void DeviceObjectOpr::update_user_machine()
{

}

// connect_type: lan
void DeviceObjectOpr::read_local_machine_from_config()
{

}

void DeviceObjectOpr::save_local_machine_to_config()
{

}

void DeviceObjectOpr::get_local_machine(map<string, DeviceObject*>& macList)
{
    macList.insert(m_scan_devices.begin(), m_scan_devices.end());
}

bool DeviceObjectOpr::set_selected_machine(const string &dev_id, bool need_disconnect/* = false*/)
{
    BOOST_LOG_TRIVIAL(info) << "set_selected_machine=" << dev_id;
    map<string, DeviceObject *> my_machine_list;
    get_my_machine_list(my_machine_list);
    auto it = my_machine_list.find(dev_id);

    if (it != my_machine_list.end()) {
        DeviceObject *devObj = it->second;
        com_id_t id = MultiComMgr::inst()->addLanDev(*devObj->get_lan_dev_info(), devObj->get_user_access_code());
        m_dev_connect_map.emplace(dev_id, id);

        m_selected_machine = dev_id;
    }
    return true;
}

void DeviceObjectOpr::unbind_machine(DeviceObject* obj)
{
    if (obj == nullptr) {
        return;
    }

    obj->set_access_code("");
    obj->erase_user_access_code();
    auto it = m_dev_connect_map.find(obj->get_dev_id());
    if (it != m_dev_connect_map.end()) {
        MultiComMgr::inst()->removeLanDev(it->second);
    }

    auto devIt = m_user_devices.find(obj->get_dev_id());
    if (devIt != m_user_devices.end()) {
        delete devIt->second;
        m_user_devices.erase(devIt);
    }
}

void DeviceObjectOpr::get_my_machine_list(map<string, DeviceObject*>& devList)
{
    for (auto it = m_user_devices.begin(); it != m_user_devices.end(); it++) {
        if (!it->second)
            continue;
        devList.insert(make_pair(it->first, it->second));
    }

    for (auto it = m_scan_devices.begin(); it != m_scan_devices.end(); it++) {
        if (!it->second)
            continue;
        if (it->second->has_access_right()) {
            // remove redundant in userMachineList
            if (devList.find(it->first) == devList.end()) {
                devList.emplace(make_pair(it->first, it->second));
            }
        }
    }

    for (auto it = m_local_devices.begin(); it != m_local_devices.end(); it++) {
        if (devList.find(it->first) == devList.end()) {
            devList.emplace(make_pair(it->first, it->second));
        }
    }
}

DeviceObject* DeviceObjectOpr::get_scan_device(const string& dev_id)
{
    if (dev_id.empty())
        return nullptr;
    auto it = m_scan_devices.find(dev_id);
    if (it == m_scan_devices.end())
        return nullptr;
    return it->second;
}

void DeviceObjectOpr::onEraseAccessCode(ComConnectionExitEvent& event)
{
    DeviceObject *devObj = get_scan_device(m_selected_machine);
    if (devObj == nullptr)
        return;

    devObj->erase_user_access_code();
}

void DeviceObjectOpr::onUpdateUserMachine(ComConnectionReadyEvent& event)
{
    DeviceObject *devObj = get_scan_device(m_selected_machine);
    if (devObj == nullptr)
        return;

    if (m_user_devices.find(m_selected_machine) == m_user_devices.end()) {
        DeviceObject *userObj = new DeviceObject(*devObj);
        userObj->set_online_state(true);
        m_user_devices.emplace(make_pair(m_selected_machine, userObj));
    }
}



}

}