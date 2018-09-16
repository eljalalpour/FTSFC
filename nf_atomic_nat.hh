#ifndef NF_ATOMIC_NAT_HH
#define NF_ATOMIC_NAT_HH

#include <unordered_map>
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

class NFAtomicNAT : public Element {
private:
    std::unordered_map<uint32_t, bool> _fake_map;

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
#endif
