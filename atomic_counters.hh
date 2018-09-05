#pragma once

#include <click/element.hh>
#include <atomic>

#define DEFAULT_SIZE 64

CLICK_DECLS

class AtomicCounters : public Element {
public:
    std::atomic_int counters[DEFAULT_SIZE];

    AtomicCounters ();

    ~AtomicCounters();

    const char *class_name() const { return "AtomicCounters"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS