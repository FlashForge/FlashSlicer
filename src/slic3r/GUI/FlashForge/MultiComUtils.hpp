#ifndef slic3r_GUI_MultiComUtils_hpp_
#define slic3r_GUI_MultiComUtils_hpp_

#include <string>
#include "FlashNetworkIntfc.h"
#include "MultiComDef.hpp"

namespace Slic3r { namespace GUI {

class MultiComUtils
{
public:
    static ComErrno getLanDevList(std::vector<fnet_lan_dev_info> &devInfos);

    static ComErrno getTokenByPassword(const std::string &userName, const std::string &password,
        com_token_data_t &tokenData);

    static ComErrno refreshToken(const std::string &refreshToken, com_token_data_t &tokenData);

    static ComErrno fnetRet2ComErrno(int networkRet);
};

}} // namespace Slic3r::GUI

#endif
