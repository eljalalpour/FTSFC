#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "transmitter.hh"
#include <unordered_map>

CLICK_DECLS

#define DEFAULT_INDEX 0
#define DEFAULT_VALUE 1

class TFNAT : public Element {
private:
    std::unordered_map<uint32_t, bool> _fake_map;
    bool _trans_init;

    Transmitter *_trans;

    inline void _init_transmitter();

public:
    TFNAT  ();

    ~TFNAT ();

    const char *class_name() const { return "TFNAT "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    inline bool bad_header(const click_ip *iph);

    inline uint32_t flow_id(Packet *p);
};

CLICK_ENDDECLS