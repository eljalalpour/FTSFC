#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

#define DEFAULT_INDEX   0

class NFLockFreeCounter : public Element {
private:
    int _index;
    bool _init_state;
    LockFreeArray* _shared_state;
    inline void _init_shared_state();

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