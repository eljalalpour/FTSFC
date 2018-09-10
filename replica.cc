#include <click/config.h>
#include <click/router.hh>
#include "replica.hh"

CLICK_DECLS

Replica::Replica() { }

Replica::~Replica() { }

inline void Replica::_init_shared_state_pointer() {
    if (!_shared_state_init) {
        _shared_state_init = true;

        Router *r = this->router();
        _shared_state = (SharedLockFreeState *)(r->find("shared_state"));
    }//if
}

void Replica::push(int source, Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Replica");

    _init_shared_state_pointer();

    if (source == INPUT_PORT_TO_PROCESS) { // Admitting a new packet
        try {
            _shared_state->process_piggyback_message(p);

            output(OUTPUT_PORT_TO_MIDDLEBOX).push(p);
        }//try
        catch(...) {
            p->kill();
        }//catch
    }//if
    else { // Forwarding the packet to next replica
        _shared_state->_log_in_operation_state();
        _shared_state->_construct_piggyback_message(p);

        output(OUTPUT_PORT_TO_NEXT_MIDDLEBOX).push(p);
    }//else

    DEBUG("End Replica");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Replica)
