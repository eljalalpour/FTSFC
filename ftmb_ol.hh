#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

class FTMBOutputLogger : public Element {
private:
    long _per_packet_latency; // In micro-seconds
    long _loop_count;

public:
    FTMBOutputLogger ();

    ~FTMBOutputLogger();

    const char *class_name() const { return "FTMBOutputLogger"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS