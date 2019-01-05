#pragma once

#include <unordered_map>
#include <click/config.h>
#include <click/element.hh>
#include "shared_state.hh"

CLICK_DECLS

class NAT : public Element {
private:
    bool _init_array;
    SharedState* _lock_free_array;

    std::unordered_map<uint32_t, bool> _fake_map;

    inline void _init_shared_state();

public:
    NAT ();

    ~NAT();

    const char *class_name() const { return "NAT"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    int configure(Vector<String> &, ErrorHandler *) CLICK_COLD;

    inline bool bad_header(const click_ip *iph);

    inline uint32_t flow_id(Packet *p);
};

CLICK_ENDDECLS

