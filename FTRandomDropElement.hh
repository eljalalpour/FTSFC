#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "FTTypes.hh"

CLICK_DECLS

/**
 * This element drops packet randomly
 */
class FTRandomDropElement: public Element {
private:
    double _dropChance;

public:
    FTRandomDropElement();
    ~FTRandomDropElement();

    const char *class_name() const { return "FTRandomDropElement"; }
    const char *port_count() const { return PORTS_1_1; }
    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);
    void push(int, Packet *p);
};

CLICK_ENDDECLS