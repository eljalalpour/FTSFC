#pragma once

#include <click/element.hh>
#include <click/atomic.hh>

#define DEFAULT_SIZE 64

CLICK_DECLS

class AtomicCounters : public Element {
public:
    atomic_uint32_t counters[DEFAULT_SIZE];

    AtomicCounters ();

    ~AtomicCounters();

    const char *class_name() const { return "AtomicCounters"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS