#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

#define MS2US 1000
#define US2NS 1000
#define MS2NS 1000000

class FTMBOutputLogger : public Element {
private:
    long _per_packet_latency; // In micro-seconds
    long _loop_count;

public:
    FTMBOutputLogger ();

    ~FTMBOutputLogger();

    int configure(Vector<String> &conf, ErrorHandler *errh);

    const char *class_name() const { return "FTMBOutputLogger"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS