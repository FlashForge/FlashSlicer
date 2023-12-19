#ifndef _FLASHNETWORK_H_
#define _FLASHNETWORK_H_

#ifdef _WIN32
#ifdef _FNET_EXPORT_
#define FNET_API __declspec(dllexport)
#else
#define FNET_API __declspec(dllimport)
#endif
#else
#define FNET_API
#endif


#pragma pack(push, 4)

#define MAX_DEVICE_ID_LEN 128
#define MAX_DEVICE_NAME_LEN 128

typedef struct fnet_lan_dev_info {
    char id[MAX_DEVICE_ID_LEN];
    char name[MAX_DEVICE_NAME_LEN];
    char ip[16];
    unsigned short port;
    unsigned short vid;
    unsigned short pid;
    unsigned short connectMode; // 0 lan mode, 1 wan mode
} fnet_lan_dev_info_t;

typedef struct fnet_token_info {
    int expiresIn;
    char *accessToken;
    char *refreshToken;
} fnet_token_info_t;

typedef struct fnet_wan_dev_info {
    char *id;
    char *name;
    char *model;
    char *imageUrl;
    char *status;
    char *location;
} fnet_wan_dev_info_t;

typedef struct fnet_wan_dev_status {
    //
} fnet_wan_dev_status_t;

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

FNET_API int fnet_initlize();

FNET_API void fnet_uninitlize();

FNET_API const char *fnet_getVersion();

FNET_API int fnet_getLanDevList(fnet_lan_dev_info_t **infos, int *devCnt);

FNET_API void fnet_freeLanDevInfos(fnet_lan_dev_info_t *infos);

FNET_API int fnet_getTokenByPassword(const char *userName, const char *password,
    fnet_token_info_t **tokenInfo);

FNET_API int fnet_refreshToken(const char *refreshToken, fnet_token_info_t **tokenInfo);

FNET_API void fnet_freeTokenInfo(fnet_token_info_t *tokenInfo);

FNET_API int fnet_getWanDevList(const char *accessToken, fnet_wan_dev_info_t **infos,
    int *devCnt);

FNET_API void fnet_freeWanDevList(fnet_wan_dev_info_t *infos, int devCnt);

FNET_API int fnet_getWanDevStatus(const char *accessToken, const char *devId,
    fnet_wan_dev_status_t **status);

FNET_API void fnet_freeWanDevStatus(fnet_wan_dev_status_t *status);

#ifdef __cplusplus
}
#endif

#endif // !_FLASHNETWORK_H_
