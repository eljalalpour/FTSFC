#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

#define DEFAULT_INDEX   0

class Decoder : public Element {
private:
    Util _util;
    // TODO: concurrency control on _msg
    PiggyBackMessage _msg;

public:
    Decoder ();

    ~Decoder();

    const char *class_name() const { return "Decoder"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS