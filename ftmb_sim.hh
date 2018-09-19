#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <click/timestamp.hh>

CLICK_DECLS

#define MS2US   1000

class FTMBSim : public Element {
private:
    int _period;
    int _delay;
    bool _first_packet_seen;

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