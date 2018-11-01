#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "lock_free_array.hh"

CLICK_DECLS

class FTMBMaster : public Element {
private:
    long _period; // In mili-seconds
    long _delay; // In mili-seconds
    long _per_packet_latency; // In micro-seconds
    long _loop_count;
    bool _first_packet_seen;

    bool _init_state;
    LockFreeArray* _shared_state;
    inline void _init_shared_state();

    std::chrono::time_point<std::chrono::high_resolution_clock> _last_snapshot_timestamp;

public:
    FTMBMaster ();

    ~FTMBMaster();

    const char *class_name() const { return "FTMBMaster"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS