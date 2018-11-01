#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

class FTMBInputLogger : public Element {
public:
    FTMBInputLogger ();

    ~FTMBInputLogger();

    const char *class_name() const { return "FTMBInputLogger"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS