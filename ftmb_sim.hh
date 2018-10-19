#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <click/timestamp.hh>
#include "nf_lock_free_counter.hh"

CLICK_DECLS

#define MS2US               1000
#define MAX_FIRST_DELAY_MS  20

class FTMBSim : public Element {
private:
    int _period;
    int _delay;
    int _first_delay;
    bool _first_packet_seen;

    bool _init_state;
    LockFreeArray* _shared_state;
    inline void _init_shared_state();

    Timestamp _last_snapshot_timestamp;

public:
    FTMBSim ();

    ~FTMBSim();

    const char *class_name() const { return "FTMBSim"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS