#pragma once

#include <deque>
#include "shared_state_base.hh"

using std::deque;

#define COARSE_GRAINED_VOR_LOCK
#define FINE_GRAINED_VOR_LOCK

#define PAD          0
#define VOR_PKT_SIZE DEFAULT_OFFSET + sizeof(VectorOfClocks) + PAD
#define PAL_PKT_SIZE DEFAULT_OFFSET + sizeof(PacketAccessLog) + PAD

enum FTMBOperation {
    AccessSharedVariable,
};

class FTMBSharedState: SharedStateBase {
public:
    FTMBSharedState ();

    ~FTMBSharedState();

    const char *class_name() const { return "FTMBSharedState"; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    inline Locker* preprocess(int16_t, int16_t);

    inline void postprocess(int16_t, Locker*, Packet*, Element::Port&);

protected:
    int _queues;
    VectorOfClocks _maxs; /// MAX in FTMB's definition
//    Locker _maxs_lockers[MAX_QUEUES]; /// Locks for _maxs
    std::deque<FTMBSeqNumber> _counters; /// s_ij in FTMB's definition, must be equal to the number of
                                         /// variables changed per thread

    PacketAccessLog[MAX_QUEUES] _pals;

    virtual inline Locker* get_locker(int16_t, int16_t, FTMBOperation);

private:
    inline void _read_vor(VectorOfClocks &);

    inline void _transfer(VectorOfClocks &, Packet *, Element::Port&);

    inline void _fix_header(Packet*);
};

Locker* FTMBSharedState::preprocess(int16_t var_id, int16_t queue) {
    Locker* locker = get_locker(var_id, queue, FTMBOperation::AccessSharedVariable);
    LOCK(locker);

    /// increment the counter of shared variable
    ++_counters[var_id]

    /// store PAL
    _pals[queue].set(var_id, _maxs[queue]++);

    return locker;
}

void FTMBSharedState::_read_vor(VectorOfClocks& vor) {
    //TODO: check if locking is required or not
    std::memcpy(vor, _maxs, sizeof(VectorOfClocks));
}

void FTMBSharedState::_fix_header(Packet* p) {
    click_ip *ip = reinterpret_cast<click_ip *>(p->data() + MAC_HEAD_SIZE);
    click_udp *udp = reinterpret_cast<click_udp *>(ip + UDP_HEAD_OFFSET_AFTER_MAC_HEAD);

    udp->uh_ulen = htons(p->length() - MAC_HEAD_SIZE - sizeof(click_ip));
    udp->uh_sum = DEFAULT_CRC;
    ip->ip_hl = sizeof(click_ip) >> 2;
    ip->ip_len = htons(p->length() - MAC_HEAD_SIZE);
    ip->ip_sum = DEFAULT_CRC;
}

void FTMBSharedState::_transfer(int16_t queue, VectorOfClocks& vor, Packet* p, Element::Port& output_port) {
    // PAL packet
    auto pal_pkt = Packet::make(CAST_AWAY_PACKET_DATA(p), PAL_PKT_SIZE, Util::no_op_pkt_destructor);
    std::memcpy(pal_pkt->data(), _pals[queue], sizeof(PacketAccessLog));
    _fix_header(pal_pkt);
    output_port.push(pal_pkt);

    // VOR packet
    auto vor_pkt = Packet::make(CAST_AWAY_PACKET_DATA(p), VOR_PKT_SIZE, Util::no_op_pkt_destructor);
    std::memcpy(vor_pkt->data(), vor, sizeof(VectorOfClocks));
    _fix_header(vor_pkt);
    output_port.push(vor_pkt);

}

void FTMBSharedState::postprocess(int16_t queue, Locker* locker, Packet* p, Element::Port& output_port) {
    /// create vector of clocks
    VectorOfClocks vor;
    read_vor(vor);

    /// release the lock
    UNLOCK(locker);

    /// transfer vor and pals
    transfer(queue, vor, p, output_port);
}

Locker* FTMBSharedState::get_locker(int16_t var_id, int16_t queue, FTMBOperation op) {
    Locker* locker = nullptr;

    switch (op) {
        case FTMBOperation::AccessSharedVariable:
            locker = _shared_locks->locker_ptr(var_id);
            break;
    }//switch

    return locker;
}
