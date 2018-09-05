#pragma once

#include <click/config.h>
#include <click/element.hh>
#inculde <atomic>
#inculde <vector>

#define DEFAULT_SIZE 64

CLICK_DECLS

class AtomicCounters : public Element {
public:
    atomic_int _counters[DEFAULT_SIZE];

    AtomicCounters () { };

    ~AtomicCounters() { };

    const char *class_name() const { return "AtomicCounters"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p) { return p; }
};

CLICK_ENDDECLS