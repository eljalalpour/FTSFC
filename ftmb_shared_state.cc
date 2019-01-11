#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include <cstring>
#include "ftmb_shared_state.hh"
#include <click/glue.hh>

CLICK_DECLS

FTMBSharedState::FTMBSharedState() { };

FTMBSharedState::~FTMBSharedState() { };

int FTMBSharedState::configure(Vector <String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("QUEUES", _queues)
                .read_mp("SRCETH", EtherAddressArg(), _ethh.ether_shost)
                .read_mp("DSTETH", EtherAddressArg(), _ethh.ether_dhost)
                .consume() < 0)
        return -1;

    LOG("FTMB Shared state, queues: %d", _queues);

    //TODO: initialize packets for PALs, for VOR to be sent,

    auto result = SharedStateBase::configure(conf, errh);
    if (result != -1) {
        /// init counters, one per shared variable
        _counters = std::deque<FTMBSeqNumber>(_shared_locks->size());
    }//if

    return result;
}

int FTMBSharedState::initialize(ErrorHandler *) {
    WritablePacket *q = Packet::make(PAL_PKT_SIZE);
    _pal_pkt = q;
    _ethh.ether_type = htons(0x0800);
    memcpy(q->data(), &_ethh, 14);
    click_ip *ip = reinterpret_cast<click_ip *>(q->data()+14);
    click_udp *udp = reinterpret_cast<click_udp *>(ip + 1);

    // set up IP header
    ip->ip_v = 4;
    ip->ip_hl = sizeof(click_ip) >> 2;
    ip->ip_len = htons(_len-14);
    ip->ip_id = 0;
    ip->ip_p = IP_PROTO_UDP;
    ip->ip_src = _sipaddr;
    ip->ip_dst = _dipaddr;
    ip->ip_tos = 0;
    ip->ip_off = 0;
    ip->ip_ttl = 250;
    ip->ip_sum = 0;
    ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));
    _packet->set_dst_ip_anno(IPAddress(_dipaddr));
    _packet->set_ip_header(ip, sizeof(click_ip));

    // set up UDP header
    udp->uh_sport = htons(_sport);
    udp->uh_dport = htons(_dport);
    udp->uh_sum = 0;
    unsigned short len = _len-14-sizeof(click_ip);
    udp->uh_ulen = htons(len);
    if (_cksum) {
        unsigned csum = click_in_cksum((uint8_t *)udp, len);
        udp->uh_sum = click_in_cksum_pseudohdr(csum, ip, len);
    } else
        udp->uh_sum = 0;

    return 0;
}

Packet *FTMBSharedState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedStateBase)
EXPORT_ELEMENT(FTMBSharedState)
