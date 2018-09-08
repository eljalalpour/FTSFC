#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

#define DEFAULT_INDEX   0

class Encoder : public Element {
private:
    Util _util;
    PiggyBackMessage _msg;

public:
    Encoder ();

    ~Encoder();

    const char *class_name() const { return "Encoder"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS