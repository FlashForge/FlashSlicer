#include "FlashNetworkIntfc.h"
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace fnet {

FlashNetworkIntfc::FlashNetworkIntfc(const char *libraryPath)
    : m_isOk(false)
{
    library_handle_t libraryHandle = loadLibrary(libraryPath);
    if (libraryHandle == INVALID_LIBRARY_HANDLE) {
        return;
    }
#define INIT_FUNC_PTR(ptr, func)\
        ptr = (decltype(&func))getFuncPtr(libraryHandle, #func);\
        if (ptr == nullptr) {\
            return;\
        }
    INIT_FUNC_PTR(initlize, fnet_initlize);
    INIT_FUNC_PTR(uninitlize, fnet_uninitlize);
    INIT_FUNC_PTR(getVersion, fnet_getVersion);
    INIT_FUNC_PTR(getLanDevList, fnet_getLanDevList);
    INIT_FUNC_PTR(freeLanDevInfos, fnet_freeLanDevInfos);
    INIT_FUNC_PTR(getTokenByPassword, fnet_getTokenByPassword);
    INIT_FUNC_PTR(refreshToken, fnet_refreshToken);
    INIT_FUNC_PTR(freeTokenInfo, fnet_freeTokenInfo);
    INIT_FUNC_PTR(getWanDevList, fnet_getWanDevList);
    INIT_FUNC_PTR(freeWanDevList, fnet_freeWanDevList);
    INIT_FUNC_PTR(getWanDevStatus, fnet_getWanDevStatus);
    INIT_FUNC_PTR(freeWanDevStatus, fnet_freeWanDevStatus);
    if (initlize() == 0) {
        m_isOk = true;
    }
}

FlashNetworkIntfc::~FlashNetworkIntfc()
{
    if (m_isOk) {
        uninitlize();
    }
}

library_handle_t FlashNetworkIntfc::loadLibrary(const char *libraryPath)
{
#ifdef _WIN32
    std::vector<wchar_t> wpath(256, 0);
    ::MultiByteToWideChar(CP_UTF8, NULL, libraryPath, (int)strlen(libraryPath), wpath.data(), (int)wpath.size());
    library_handle_t handle = LoadLibraryW(wpath.data());
    if (handle == INVALID_LIBRARY_HANDLE) {
        printf("load network module error: %x\n", GetLastError());
    }
    return handle;
#else
    library_handle_t handle = dlopen(libraryPath, RTLD_LAZY);
    if (handle == nullptr) {
        const char *dllError = dlerror();
        printf("load network module error: %s\n", dllError);
    }
    return handle;
#endif
}

void *FlashNetworkIntfc::getFuncPtr(library_handle_t libraryHandle, const char *funcName)
{
#ifdef _WIN32
    void *funcPtr = GetProcAddress(libraryHandle, funcName);
    if (funcPtr == nullptr) {
        printf("can't find function %s\n", funcName);
    }
    return funcPtr;
#else
    void *funcPtr = dlsym(libraryHandle, funcName);
    if (funcPtr == nullptr) {
        printf("can't find function %s\n", funcName);
    }
    return funcPtr;
#endif
}

} // namespace fnet
