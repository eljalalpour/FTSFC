#include <click/config.h>
#include <click/handlercall.hh>
#include "shared_atomic_state.hh"

CLICK_DECLS

SharedAtomicState::SharedAtomicState () { };

SharedAtomicState::~SharedAtomicState() { };

Packet *SharedAtomicState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedAtomicState)
