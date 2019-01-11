#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "ftmb_shared_state_counter.hh"

CLICK_DECLS

class FTMBCounter : public Element {
private:
    size_t _index;
    FTMBSharedStateCounter* _shared_state;
    size_t _counter;
    String _shared_state_element_name;
    inline void _init_shared_state();

public:
    FTMBCounter ();

    ~FTMBCounter();

    const char *class_name() const { return "FTMBCounter"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
