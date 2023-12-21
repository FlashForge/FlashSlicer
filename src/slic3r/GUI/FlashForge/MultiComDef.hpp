#ifndef slic3r_GUI_MultiComDef_hpp_
#define slic3r_GUI_MultiComDef_hpp_

#include <string>
#include <vector>

namespace Slic3r { namespace GUI {

using com_id_t = int;
using com_id_list_t = std::vector<com_id_t>;
constexpr com_id_t ComInvalidId = -1;

enum ComErrno {
    COM_OK,
    COM_ERROR,
    COM_UNINITIALIZED,
};

enum ComConnectMode {
    COM_CONNECT_LAN,
    COM_CONNECT_WAN,
};

struct com_token_info_t {
    int expiresIn;
    std::string accessToken;
    std::string refreshToken;
};

struct com_dev_data_t {
    
};

}} // namespace Slic3r::GUI

#endif