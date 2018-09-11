#pragma once

#include "defs.hh"
#include "shared_lock_free_state.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

///
/// Class Replica represents the \texttt{Replica} entity.
/// This element does not manage the state and perform no concurrency controls.
/// These operations are offloaded to class SharedLockFreeState. 
///

// The input port of a packet to be processed
#define INPUT_PORT_TO_PROCESS 0

// The input port from the middlebox. The packet is processed
#define INPUT_PORT_PROCESSED  1

#define OUTPUT_PORT_TO_MIDDLEBOX 0

#define OUTPUT_PORT_TO_NEXT_MIDDLEBOX 1

class Replica : public Element {
private:
    bool _shared_state_init;
//    SharedLockFreeState* _shared_state;
    inline void _init_shared_state_pointer();

public:
    Replica ();

    ~Replica();

    const char *class_name() const { return "Replica"; }

    const char *port_count() const { return PORTS_2_2; }

    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet*);
};

CLICK_ENDDECLS