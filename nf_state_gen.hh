#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <vector>
#include "shared_state.hh"

CLICK_DECLS

class NFStateGen : public Element {
private:
    size_t _state_size;
    std::vector<int> _vect1;
    std::vector<int> _vect2;

    inline void _init_state();

public:
    NFStateGen ();

    ~NFStateGen();

    const char *class_name() const { return "NFStateGen"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS