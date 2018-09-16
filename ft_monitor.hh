#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "shared_lock_free_state.hh"

CLICK_DECLS

class FTMonitor : public Element {
private:
    bool _init_state;
    SharedLockFreeState* _shared_state;
    int _index;
    int _for_count;

    inline void _init_shared_state();

public:
    FTMonitor ();

    ~FTMonitor();

    const char *class_name() const { return "FTMonitor"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS