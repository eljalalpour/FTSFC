#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

#define DEFAULT_INDEX   0

class Forwarder : public Element {
private:
    Util _util;
    PiggyBackMessage _msg;

public:
    Forwarder ();

    ~Forwarder();

    const char *class_name() const { return "Forwarder"; }

    const char *port_count() const { return PORTS_2_1; }

    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet*);
};

CLICK_ENDDECLS