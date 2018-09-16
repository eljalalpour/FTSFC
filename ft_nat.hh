#pragma once

#include <unordered_map>
#include <click/config.h>
#include <click/element.hh>
#include "shared_lock_free_state.hh"

CLICK_DECLS

#define DEFAULT_SIZE 8

class FTNAT : public Element {
private:
    bool _init_state;
    SharedLockFreeState* _shared_state;
    int _index;
    std::unordered_map<uint32_t, bool> _fake_map;

    inline void _init_shared_state();

public:
    FTNAT ();

    ~FTNAT();

    const char *class_name() const { return "FTNAT "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &conf, ErrorHandler *errh);

    Packet *simple_action(Packet *p);

    inline bool bad_header(const click_ip *iph);

    inline uint32_t flow_id(Packet *p);
};

CLICK_ENDDECLS
