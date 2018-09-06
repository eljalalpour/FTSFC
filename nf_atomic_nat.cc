#include "atomic_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include <click/ipflowid.hh>
#include "nf_atomic_nat.hh"

CLICK_DECLS

NFAtomicNAT::NFAtomicNAT () { };

NFAtomicNAT::~NFAtomicNAT() { };

bool NFAtomicNAT::bad_header(click_ip *iph) {
    return (iph->ip_p != IP_PROTO_TCP && iph->ip_p != IP_PROTO_UDP)
           || !IP_FIRSTFRAG(iph);
}

uint32_t NFAtomicNAT::flow_id(Packet *p) {
    // Real NAT stuff!
    IPFlowID flow_id(p);
    uint64_t ip_part =
            (uint64_t)flow_id.saddr().addr() |
            ((uint64_t)flow_id.daddr().addr()) << 32;

    uint32_t pr_part =
            (uint64_t)flow_id.sport() |
            ((uint64_t)flow_id.dport()) << 16;

    uint32_t hash_val =
            ((uint32_t)std::hash<uint64_t>(ip_part) % (1 << 16)) |
            pr_part;

    // Just return some random value!
    return fast_random() % DEFAULT_SIZE;
}

Packet *NFAtomicNAT::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin NFAtomicNAT with index %d:", _index);
    Router *r = this->router();

    click_ip *iph = rp->ip_header();

    if (bad_header(iph)) {
        p->kill();
        return 0;
    }//if

    // Finding flow id
    uint32_t fid = flow_id(p);

    // Keep as is
    AtomicArray *afc = (AtomicArray *)(r->find("array"));
    if (afc->counters[fid] != 0)
        afc->counters[fid] = 1;

    LOG("End NFAtomicNAT %d:", _index);
    LOG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFAtomicNAT)
