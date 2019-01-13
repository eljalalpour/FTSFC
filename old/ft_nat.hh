#pragma once

#include "../defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "../ft_shared_state.hh"
#include <unordered_map>

CLICK_DECLS

class FTNAT : public Element {
private:
    FTSharedState* _shared_state;
    std::unordered_map<uint32_t, bool> _fake_map;

    inline void _init_shared_state();

public:
    FTNAT ();

    ~FTNAT();

    const char *class_name() const { return "FTNAT"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);

    Packet *simple_action(Packet *p);

    inline bool bad_header(const click_ip *iph);

    inline uint32_t flow_id(Packet *p);
};

CLICK_ENDDECLS
