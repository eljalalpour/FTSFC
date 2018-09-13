#include <click/config.h>
#include <click/router.hh>
#include "pm_construct.hh"

CLICK_DECLS

PMConstruct::PMConstruct() : _shared_state_init(false) { }

PMConstruct::~PMConstruct() { }

inline void PMConstruct::_init_shared_state_pointer() {
    if (!_shared_state_init) {
        _shared_state_init = true;

        Router *r = this->router();
        _shared_state = (SharedLockFreeState *)(r->find("shared_state"));
    }//if
}

Packet *PMConstruct::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin PMConstruct");

    _init_shared_state_pointer();

    _shared_state->construct_piggyback_message(p);

    DEBUG("End PMConstruct");
    DEBUG("--------------------");

//    PiggybackMessage* pm = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
//
//    Util util;
//    LOG("After construction:");
//    util.print(*pm[0]);
//    util.print(*pm[1]);

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PMConstruct)
