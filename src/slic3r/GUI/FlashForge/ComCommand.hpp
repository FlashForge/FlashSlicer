#ifndef slic3r_GUI_ComCommand_hpp_
#define slic3r_GUI_ComCommand_hpp_

#include "FlashNetworkIntfc.h"
#include "MultiComDef.hpp"

namespace Slic3r { namespace GUI {

class ComCommand
{
public:
    ComCommand(int id = -1) : m_id(id) {
    }
    virtual ~ComCommand() {
    }
    virtual bool isDup(const ComCommand *that) {
        return typeid(*this) == typeid(*that);
    }
    virtual ComErrno exec(fnet::FlashNetworkIntfc *networkIntfc) = 0;

private:
    int m_id;
};

}} // namespace Slic3r::GUI

#endif
