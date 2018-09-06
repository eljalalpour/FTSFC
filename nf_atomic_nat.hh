#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <atomic>
#include <vector>

CLICK_DECLS

#define DEFAULT_SIZE 65536

class NFAtomicNAT : public Element {
public:
    NFAtomicNAT ();

    ~NFAtomicNAT();

    const char *class_name() const { return "NFAtomicNAT "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    inline bool bad_header(const click_ip *iph);

    inline uint32_t flow_id(Packet *p);
};

CLICK_ENDDECLS