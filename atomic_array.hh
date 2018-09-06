#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <atomic>

#define DEFAULT_SIZE 65536

CLICK_DECLS

class AtomicArray : public Element {
public:
    std::atomic_int counters[DEFAULT_SIZE];

    AtomicArray ();

    ~AtomicArray();

    const char *class_name() const { return "AtomicArray"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS