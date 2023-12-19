#ifndef _FNET_FLASHNETWORKINTFC_H_
#define _FNET_FLASHNETWORKINTFC_H_

#include "FlashNetwork.h"

#ifdef _WIN32
#include <Windows.h>
typedef HMODULE library_handle_t;
#define INVALID_LIBRARY_HANDLE NULL
#else
typedef void *library_handle_t;
#define INVALID_LIBRARY_HANDLE nullptr
#endif // _WIN32

namespace fnet {

class FlashNetworkIntfc
{
public:
    decltype(&fnet_initlize) initlize;
    decltype(&fnet_uninitlize) uninitlize;
    decltype(&fnet_getVersion) getVersion;
    decltype(&fnet_getLanDevList) getLanDevList;
    decltype(&fnet_freeLanDevInfos) freeLanDevInfos;
    decltype(&fnet_getTokenByPassword) getTokenByPassword;
    decltype(&fnet_refreshToken) refreshToken;
    decltype(&fnet_freeTokenInfo) freeTokenInfo;
    decltype(&fnet_getWanDevList) getWanDevList;
    decltype(&fnet_freeWanDevList) freeWanDevList;
    decltype(&fnet_getWanDevStatus) getWanDevStatus;
    decltype(&fnet_freeWanDevStatus) freeWanDevStatus;

public:
    FlashNetworkIntfc(const char *libraryPath);

    ~FlashNetworkIntfc();

    bool isOk() const { return m_isOk; }

private:
    library_handle_t loadLibrary(const char *libraryPath);

    void *getFuncPtr(library_handle_t libraryHandle, const char *funcName);

private:
    bool m_isOk;
};

} // namespace fnet

#endif
