#include <click/config.h>
#include <click/router.hh>
#include "pm_process.hh"

CLICK_DECLS

PMProcess::PMProcess() : _shared_state_init(false) { }

PMProcess::~PMProcess() { }

inline void PMProcess::_init_shared_state_pointer() {
    if (!_shared_state_init) {
        _shared_state_init = true;

        Router *r = this->router();
        _shared_state = (SharedLockFreeState *)(r->find("shared_state"));
    }//if
}

Packet* PMProcess::simple_action(Packet *p) {
//    DEBUG("--------------------");
//    DEBUG("Begin PMProcess");
//
//    LOG("Before init");
//    _init_shared_state_pointer();
//
//    LOG("Before try");
//    try {
//        _shared_state->process_piggyback_message(p);
//    }//try
//    catch(...) {
//        p->kill();
//    }//catch
//    LOG("After try");
//
//    DEBUG("End PMProcess");
//    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PMProcess)
