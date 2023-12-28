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

#define MAX_DEVICE_SN_LEN 128
#define MAX_DEVICE_NAME_LEN 128

typedef struct fnet_lan_dev_info {
    char serialNumber[MAX_DEVICE_SN_LEN];
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
    char *devId;
    char *name;
    char *model;
    char *imageUrl;
    char *status;
    char *location;
    char *serialNumber;
} fnet_wan_dev_info_t;

typedef struct fnet_dev_detail {
    char *model;
    int nozzleCnt;
    int nozzleStyle;            // 0 independent, 1 non-independent
    char *measure;
    char *nozzleModel;
    char *firmwareVersion;
    char *macAddr;
    char *ipAddr;
    char *name;
    char *location;
    char *printStatus;
    char *printFileName;
    char *printFileThumbUrl;
    int printLayer;
    int targetPrintLayer;
    double printProgress;       // percent
    double rightTemp;
    double rightTargetTemp;
    double leftTemp;
    double leftTargetTemp;
    double platTemp;
    double platTargetTemp;
    double chamberTemp;
    double chamberTargetTemp;
    double fillAmount;          // percent
    double zAxisCompensation;   // mm
    char *rightFilamentType;
    char *leftFilamentType;
    double currentPrintSpeed;   // mm/s
    double printSpeedAdjust;    // percent
    double printDuration;       // second
    double estimatedTime;       // second
    double estimatedRightLen;   // mm
    double estimatedLeftLen;    // mm
    double coolingFanSpeed;     // percent
    double chamberFanSpeed;     // percent
    char *internalFanStatus;
    char *externalFanStatus;
    char *doorStatus;
    char *lightStatus;
    char *autoShutdown;
    double autoShutdownTime;    // minute
    double tvoc;
    double remainingDiskSpace;  // GB
    double cumulativePrintTime; // minute
    double cumulativeFilament;  // mm
    char *cameraStreamUrl;
    char *polarRegisterCode;
    char *flashRegisterCode;
    char *errorCode;
} fnet_dev_detail_t;

#pragma pack(pop)


#define FNET_OK 0
#define FNET_ERROR -1
#define FNET_VERIFY_LAN_DEV_FAILED 1

#ifdef __cplusplus
extern "C" {
#endif

FNET_API int fnet_initlize();

FNET_API void fnet_uninitlize();

FNET_API const char *fnet_getVersion();

FNET_API int fnet_getLanDevList(fnet_lan_dev_info_t **infos, int *devCnt);

FNET_API void fnet_freeLanDevInfos(fnet_lan_dev_info_t *infos);

FNET_API int fnet_getLanDevDetail(const char *ip, unsigned short port, const char *serialNumber,
    const char *checkCode, fnet_dev_detail_t **detail);

FNET_API void fnet_freeDevDetail(fnet_dev_detail_t *detail);

FNET_API int fnet_getTokenByPassword(const char *userName, const char *password,
    fnet_token_info_t **tokenInfo);

FNET_API int fnet_refreshToken(const char *refreshToken, fnet_token_info_t **tokenInfo);

FNET_API void fnet_freeTokenInfo(fnet_token_info_t *tokenInfo);

FNET_API int fnet_getWanDevList(const char *accessToken, fnet_wan_dev_info_t **infos,
    int *devCnt);

FNET_API void fnet_freeWanDevList(fnet_wan_dev_info_t *infos, int devCnt);

FNET_API int fnet_getWanDevDetail(const char *accessToken, const char *devId,
    fnet_dev_detail_t **detail);

#ifdef __cplusplus
}
#endif

#endif // !_FLASHNETWORK_H_
