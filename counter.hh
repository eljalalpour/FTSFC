#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "shared_state.hh"

CLICK_DECLS

class Counter : public Element {
private:
    size_t _index;
    SharedState* _shared_state;
    String _shared_array_element_name;
    inline void _init_shared_state();

public:
    Counter ();

    ~Counter();

    const char *class_name() const { return "Counter"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS