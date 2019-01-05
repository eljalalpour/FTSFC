#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "shared_state.hh"

CLICK_DECLS

#define MS2US 1000
#define US2NS 1000
#define MS2NS 1000000


class FTMBMaster : public Element {
private:
    long _period; // In mili-seconds
    long _delay; // In mili-seconds
    long _per_packet_latency; // In micro-seconds
    long _loop_count;
    bool _first_packet_seen;

    SharedArray* _shared_state;
    String _shared_array_element_name;
    inline void _init_shared_state();

    std::chrono::time_point<std::chrono::high_resolution_clock> _last_snapshot_timestamp;

public:
    FTMBMaster ();

    ~FTMBMaster();

    int configure(Vector<String> &conf, ErrorHandler *errh);

    const char *class_name() const { return "FTMBMaster"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS