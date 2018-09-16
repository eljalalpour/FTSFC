#include "defs.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include <click/ipflowid.hh>
#include "ft_nat.hh"

CLICK_DECLS

FTNAT::FTNAT () : _init_state(false)  { };

FTNAT::~FTNAT() { };

bool FTNAT::bad_header(const click_ip *iph) {
//    return (iph->ip_p != IP_PROTO_TCP && iph->ip_p != IP_PROTO_UDP)
//           || !IP_FIRSTFRAG(iph);

    return true;
}

void FTNAT::_init_shared_state() {
//    if (!_init_state) {
//        Router *r = this->router();
//        _shared_state = (SharedLockFreeState *)(r->find("shared_state"));
//        _init_state = true;
//    }//if
}

int FTNAT::configure(Vector<String> &conf, ErrorHandler *errh) {
//    // set index param
//    if (Args(conf, this, errh)
//                .read("INDEX", _index)
//                .complete() < 0)
//        return -1;
//
//    DEBUG("FTNAT index is %d!\n", _index);

    return 0;
}

uint32_t FTNAT::flow_id(Packet *p) {
    // Simulating Real NAT stuff!
//    IPFlowID flow_id(p);
//    uint64_t ip_part =
//            (uint64_t)flow_id.saddr().addr() |
//            ((uint64_t)flow_id.daddr().addr() << 32);
//
//    uint32_t pr_part =
//            (uint64_t)flow_id.sport() |
//            ((uint64_t)flow_id.dport()) << 16;
//
//    std::hash<uint64_t> hasher;
//    uint32_t hash_val =
//            ((uint32_t)hasher(ip_part) % (1 << 16)) |
//            pr_part;
//
//    auto it = _fake_map.find(hash_val);
//    if (it == _fake_map.end())
//        _fake_map[hash_val] = 1;
//
//    // Just return some random value!
//    return hash_val % STATE_LEN;

    return 0;
}

Packet *FTNAT::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTNAT with index %d:", _index);

//    _init_shared_state();
//
//    const click_ip *iph = p->ip_header();
//
//    if (bad_header(iph)) {
//        p->kill();
//        return 0;
//    }//if
//
//    // Finding flow id
//    uint32_t fid = flow_id(p);
//
//    if (_shared_state->inoperation[fid] == 0)
//        _shared_state->inoperation[fid] = 1;

    DEBUG("End FTNAT");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTNAT)
