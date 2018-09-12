#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "shared_lock_free_state.hh"

CLICK_DECLS

///
/// Class PMProcess performs the piggyback message process
/// This element does not manage the state and perform no concurrency controls.
/// These operations are offloaded to class SharedLockFreeState. 
///

class PMProcess : public Element {
private:
    bool _shared_state_init;
    SharedLockFreeState* _shared_state;
    inline void _init_shared_state_pointer();

public:
    PMProcess ();

    ~PMProcess();

    const char *class_name() const { return "PMProcess"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *)
};

CLICK_ENDDECLS