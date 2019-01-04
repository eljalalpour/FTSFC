#ifndef NF_NAT_HH
#define NF_NAT_HH

#include <unordered_map>
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

class NFNAT : public Element {
private:
    bool _init_array;
    SharedArray* _lock_free_array;

    std::unordered_map<uint32_t, bool> _fake_map;

    inline void _init_shared_state();

public:
    NFNAT ();

    ~NFNAT();

    const char *class_name() const { return "NFNAT"; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    inline bool bad_header(const click_ip *iph);

    inline uint32_t flow_id(Packet *p);
};

CLICK_ENDDECLS
#endif
