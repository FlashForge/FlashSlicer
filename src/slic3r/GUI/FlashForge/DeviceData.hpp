#ifndef slic3r_DeviceData_hpp_
#define slic3r_DeviceData_hpp_

#include "nlohmann/json.hpp"
#include "FlashNetwork.h"

using namespace nlohmann;
using namespace std;

namespace Slic3r {

enum ConnectMode {
    UNKNOW_MODE = -1,
    LAN_MODE,
    WAN_MODE,
};

class DeviceObject
{
public:
    DeviceObject(fnet_lan_dev_info *devInfo);

    bool        is_lan_mode_printer();
    bool        has_access_right();
    string      get_access_code();

    void        set_user_access_code(const string& code, bool only_refresh = true);
    string      get_user_access_code();
    void        erase_user_access_code();

    string      get_dev_name();

    static bool is_in_printing_status(const string& status);
    void set_print_state(const string &status);

    /* common apis */
    bool is_in_printing();

private:
    fnet_lan_dev_info *m_devInfo;
    string             m_dev_id;

    string m_user_access_code;

    /* printing status */
    string m_printStatus; /* enum string: FINISH, SLICING, RUNNING, PAUSE, INIT, FAILED */
};

}



#endif