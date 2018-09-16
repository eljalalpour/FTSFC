#ifndef FT_NAT_HH
#define FT_NAT_HH

#include <unordered_map>
#include <click/config.h>
#include <click/element.hh>
//#include "shared_lock_free_state.hh"

CLICK_DECLS

class FTNAT : public Element {
private:
    bool _init_state;
//    SharedLockFreeState* _shared_state;
    int _index;
    std::unordered_map<uint32_t, bool> _fake_map;

    inline void _init_shared_state();

public:
    FTNAT ();

    ~FTNAT();

    const char *class_name() const { return "FTNAT "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    inline bool bad_header(const click_ip *iph);

    inline uint32_t flow_id(Packet *p);
};

CLICK_ENDDECLS
#endif