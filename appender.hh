#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

#define DEFAULT_INDEX   0

class Appender : public Element {
public:
    Appender ();

    ~Appender();

    const char *class_name() const { return "Appender"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    void reserve_state_space(Packet* p);
};

CLICK_ENDDECLS