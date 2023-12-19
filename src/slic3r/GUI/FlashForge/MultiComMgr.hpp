#ifndef slic3r_GUI_MultiComMgr_hpp_
#define slic3r_GUI_MultiComMgr_hpp_

#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <boost/bimap.hpp>
#include "ComConnection.hpp"
#include "FlashNetworkIntfc.h"
#include "MultiComDef.hpp"
#include "Singleton.hpp"

namespace Slic3r { namespace GUI {

class MultiComMgr : public Singleton<MultiComMgr>
{
public:
    MultiComMgr();

    bool initalize(const std::string &newtworkDllPath);

    void uninitalize();

    ComErrno getLanDevList(std::vector<fnet_lan_dev_info> &devInfos);
    
    ComErrno addLanDev(const fnet_lan_dev_info &devInfo);

    ComErrno getTokenByPassword(const std::string &userName, const std::string &password,
        com_token_info_t &tokenInfo);

    ComErrno refreshToken(const std::string &refreshToken, com_token_info_t &tokenInfo);

    ComErrno addWanDevList(const std::string &accessToken);

    com_id_list_t getReadyDevList();

    const com_dev_data_t &devData(com_id_t &id, bool *valid = nullptr);

private:
    typedef std::shared_ptr<ComConnection> com_ptr_t;
    typedef std::list<com_ptr_t> com_ptrs_t;
    typedef std::set<com_id_t> com_id_set_t;
    typedef std::map<com_id_t, com_dev_data_t> com_dat_map_t;
    typedef std::unique_ptr<fnet::FlashNetworkIntfc> network_intfc_ptr_t;
    typedef boost::bimap<com_id_t, ComConnection*> com_ptr_map_t;
    typedef boost::bimap<com_id_t, ComConnection*>::value_type com_ptr_map_val_t;

    void initConnection(const com_ptr_t &comPtr);

    void uninitConnection(ComConnection *comConnection);

private:
    int                 m_idNum;
    com_ptrs_t          m_comPtrs;
    com_ptr_map_t       m_ptrMap;
    com_dat_map_t       m_datMap;
    com_id_set_t        m_readyIdSet;
    network_intfc_ptr_t m_networkIntfc;
};

}} // namespace Slic3r::GUI

#endif
