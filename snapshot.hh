#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <click/timestamp.hh>

CLICK_DECLS

#define MS2US 1000
#define US2NS 1000
#define MS2NS 1000000

class Snapshot : public Element {
private:
    long _period; // In mili-seconds
    long _delay; // In mili-seconds
    long _loop_count;
    bool _first_packet_seen;

    std::chrono::time_point<std::chrono::high_resolution_clock> _last_snapshot_timestamp;

public:
    Snapshot ();

    ~Snapshot();

    const char *class_name() const { return "Snapshot"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS