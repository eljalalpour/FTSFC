#pragma once

#include <click/config.h>
#include <click/element.hh>

#define DEFAULT_SIZE 65536

CLICK_DECLS

class LockFreeArray : public Element {
public:
    int counters[DEFAULT_SIZE];

    LockFreeArray ();

    ~LockFreeArray();

    const char *class_name() const { return "LockFreeArray"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    void add_handlers() CLICK_COLD;

private:
    static String read_handler(Element *, void *) CLICK_COLD;
};

CLICK_ENDDECLS
