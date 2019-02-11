#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <click/timestamp.hh>
#include "ftmb_shared_state_counter.hh"

CLICK_DECLS

#define MS2US 1000
#define US2NS 1000
#define MS2NS 1000000


class FTMBSnapshotCounter : public Element {
private:
    long _period; // In mili-seconds
    long _delay; // In mili-seconds
    long _loop_count;
    bool _first_packet_seen;

    std::chrono::time_point<std::chrono::high_resolution_clock> _last_snapshot_timestamp;

    size_t _index;
    FTMBSharedStateCounter* _shared_state;
    size_t _counter;
    String _shared_state_element_name;
    inline void _init_shared_state();

public:
    FTMBSnapshotCounter ();

    ~FTMBSnapshotCounter();

    const char *class_name() const { return "FTMBSnapshotCounter"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
