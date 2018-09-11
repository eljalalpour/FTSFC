#pragma once

#include "../defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

#define DEFAULT_INDEX   0

class FTLockFreeCounter : public Element {
private:
    int _index;

public:
    FTLockFreeCounter ();

    ~FTLockFreeCounter();

    const char *class_name() const { return "FTLockFreeCounter"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS