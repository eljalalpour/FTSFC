#include "shared_array.hh"
#include "defs.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include <click/ipflowid.hh>
#include "nf_nat.hh"

CLICK_DECLS

NFNAT::NFNAT () { };

NFNAT::~NFNAT() { };

bool NFNAT::bad_header(const click_ip *iph) {
    return (iph->ip_p != IP_PROTO_TCP && iph->ip_p != IP_PROTO_UDP)
           || !IP_FIRSTFRAG(iph);
}

void NFNAT::_init_shared_state() {
    Router *r = this->router();
    _lock_free_array = (SharedArray *)(r->find("array"));
}

uint32_t NFNAT::flow_id(Packet *p) {
    // Simulating Real NAT stuff!
    IPFlowID flow_id(p);
    uint64_t ip_part =
            (uint64_t)flow_id.saddr().addr() |
            ((uint64_t)flow_id.daddr().addr() << 32);

    uint32_t pr_part =
            (uint64_t)flow_id.sport() |
            ((uint64_t)flow_id.dport()) << 16;

    std::hash<uint64_t> hasher;
    uint32_t hash_val =
            ((uint32_t)hasher(ip_part) % (1 << 16)) |
            pr_part;

    auto it = _fake_map.find(hash_val);
    if (it == _fake_map.end())
        _fake_map[hash_val] = 1;

    // Just return some random value!
    return hash_val % STATE_LEN;
}

int NFNAT::configure(Vector<String> &conf, ErrorHandler *errh)  {
    _init_shared_state();
}

Packet *NFNAT::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin NFNAT");

    const click_ip *iph = p->ip_header();

    if (bad_header(iph)) {
        p->kill();
        return 0;
    }//if

    // Finding flow id
    uint32_t fid = flow_id(p);

    // Keep as is
    if (_lock_free_array->read(fid) == 0)
        _lock_free_array->increment(fid);

    DEBUG("End NFNAT");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFNAT)
