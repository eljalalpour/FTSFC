#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <atomic>
#include <vector>

#define DEFAULT_SIZE 64

CLICK_DECLS

class LockFreeCounters : public Element {
public:
    int counters[DEFAULT_SIZE];

    LockFreeCounters () : counters {0} { };

    ~LockFreeCounters() { };

    const char *class_name() const { return "LockFreeCounters"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p) { return p; }
};

CLICK_ENDDECLS