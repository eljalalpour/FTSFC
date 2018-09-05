#pragma once

#include "FTTypes.hh"
#include <click/config.h>
#include <click/element.hh>
#include <atomic>
#include <vector>

CLICK_DECLS

#define DEFAULT_INDEX   0

class NFLockFreeCounter : public Element {
private:
    int _index;

public:
    NFLockFreeCounter ();

    ~NFLockFreeCounter();

    const char *class_name() const { return "NFLockFreeCounter"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS