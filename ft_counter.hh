#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "ft_shared_state_counter.hh"

CLICK_DECLS

class FTCounter : public Element {
private:
    size_t _index;
    FTSharedStateCounter* _shared_state;
    String _shared_state_element_name;
    inline void _init_shared_state();

public:
    FTCounter ();

    ~FTCounter();

    const char *class_name() const { return "FTCounter"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS