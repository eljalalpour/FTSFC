#include <click/config.h>
#include "shared_atomic_state.hh"
#include <click/handlercall.hh>

CLICK_DECLS

SharedAtomicState::SharedAtomicState () { };

SharedAtomicState::~SharedAtomicState() { };

Packet *SharedAtomicState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedAtomicState)
