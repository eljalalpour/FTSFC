#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include <cstring>
#include "ftmb_shared_state.hh"
#include <click/glue.hh>
#include <click/standard/alignmentinfo.hh>
#include <click/etheraddress.hh>
#include <click/error.hh>

CLICK_DECLS

FTMBSharedState::FTMBSharedState() : _pal_generated(false) { };

FTMBSharedState::~FTMBSharedState() { };

int FTMBSharedState::configure(Vector <String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("QUEUES", _queues)
                .read("BATCH", _batch)
                .read_mp("SRCETH", EtherAddressArg(), _ethh.ether_shost)
                .read_mp("DSTETH", EtherAddressArg(), _ethh.ether_dhost)
                .read_mp("SRCIP", _sipaddr)
                .read_mp("DSTIP", _dipaddr)
                .consume() < 0)
        return -1;

    _ethh.ether_type = htons(0x0800);

    LOG("FTMB Shared state, queues: %d, batch: %d", _queues, _batch);

    //TODO: initialize packets for PALs, for VOR to be sent,

    auto result = SharedStateBase::configure(conf, errh);
    memset(_maxs, 0, sizeof(_maxs));

    if (result != -1) {
        /// init counters, one per shared variable
        _counters = std::deque<FTMBSeqNumber>(_shared_locks->size(), 0);
    }//if

    return result;
}

void FTMBSharedState::_create_packet(int pkt_size, Packet** _pkt_ptr) {
    WritablePacket *q = Packet::make(pkt_size);
    *_pkt_ptr = q;
    memcpy(q->data(), &_ethh, MAC_HEAD_SIZE);
    click_ip *ip = reinterpret_cast<click_ip *>(q->data() + MAC_HEAD_SIZE);
    click_udp *udp = reinterpret_cast<click_udp *>(ip + UDP_HEAD_OFFSET_AFTER_MAC_HEAD);

    // set up IP header
    ip->ip_v = 4;
    ip->ip_hl = sizeof(click_ip) >> 2;
    ip->ip_len = htons(pkt_size - MAC_HEAD_SIZE);
    ip->ip_id = 0;
    ip->ip_p = IP_PROTO_UDP;
    ip->ip_src = _sipaddr;
    ip->ip_dst = _dipaddr;
    ip->ip_tos = 0;
    ip->ip_off = 0;
    ip->ip_ttl = 250;
    ip->ip_sum = 0;
    ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));
    (*_pkt_ptr)->set_dst_ip_anno(IPAddress(_dipaddr));
    (*_pkt_ptr)->set_ip_header(ip, sizeof(click_ip));

    // set up UDP header
    udp->uh_sport = htons(10000);
    udp->uh_dport = htons(10000);
    udp->uh_sum = 0;
    unsigned short len = pkt_size - MAC_HEAD_SIZE - sizeof(click_ip);
    udp->uh_ulen = htons(len);
    udp->uh_sum = 0;
}

int FTMBSharedState::initialize(ErrorHandler *) {
    _create_packet(PAL_PKT_SIZE, &_pal_pkt);
    _create_packet(VOR_PKT_SIZE, &_vor_pkt);

    return 0;
}

Packet *FTMBSharedState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedStateBase)
EXPORT_ELEMENT(FTMBSharedState)
