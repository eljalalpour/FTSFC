#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "ft_shared_state.hh"

CLICK_DECLS

#define DEFAULT_INDEX   0

class FTLockFreeCounter : public Element {
private:
    SharedState* _shared_state;
    int _index;

    inline void _init_shared_state();

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