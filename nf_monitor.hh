#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "shared_array.hh"

CLICK_DECLS

class NFMonitor : public Element {
private:
    SharedArray* _lock_free_array;
    size_t _index;
    int _for_count;

    inline void _init_shared_state();

public:
    NFMonitor ();

    ~NFMonitor();

    const char *class_name() const { return "NFMonitor"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS