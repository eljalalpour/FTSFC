#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "shared_lock_free_state.hh"

CLICK_DECLS

///
/// Class PMConstruct performs the piggyback message construction
/// This element does not manage the state and perform no concurrency controls.
/// These operations are offloaded to class SharedLockFreeState. 
///

class PMConstruct : public Element {
private:
    int _queue;
    SharedLockFreeState* _shared_state;
    inline void _init_shared_state_pointer();

public:
    PMConstruct ();

    ~PMConstruct();

    const char *class_name() const { return "PMConstruct"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *);
};

CLICK_ENDDECLS