#pragma once

#include <deque>
#include <cstring>
#include <click/packet_anno.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <clicknet/udp.h>
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

class FTMBSharedState: public SharedStateBase {
public:
    FTMBSharedState ();

    ~FTMBSharedState();

    const char *class_name() const { return "FTMBSharedState"; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    int initialize(ErrorHandler *);

    inline Locker* preprocess(int16_t, int16_t);

    inline void postprocess(int16_t, Locker*, Packet*, const Element::Port*);

protected:
    int _queues;
    int _batch;
    bool _pal_generated;
    VectorOfClocks _maxs; /// MAX in FTMB's definition
//    Locker _maxs_lockers[MAX_QUEUES]; /// Locks for _maxs
    std::deque<FTMBSeqNumber> _counters; /// s_ij in FTMB's definition, must be equal to the number of
    /// variables changed per thread

    PacketAccessLog _pals[MAX_QUEUES];

    virtual inline Locker* get_locker(int16_t, int16_t, FTMBOperation);

private:
    inline void _read_vor();

    inline void _transfer(int16_t, Packet*, const Element::Port*);

    Packet *_pal_pkt;
    Packet *_vor_pkt;

    click_ether _ethh;
    struct in_addr _sipaddr;
    struct in_addr _dipaddr;

    void _create_packet(int, Packet**);
};

Locker* FTMBSharedState::preprocess(int16_t var_id, int16_t queue) {
    Locker* locker = get_locker(var_id, queue, FTMBOperation::AccessSharedVariable);
    LOCK(locker);

    /// increment the counter of shared variable
    ++_counters[var_id];

    /// store PAL
    _pals[queue].set(var_id, _maxs[queue]++);
    _pal_generated = true;

    return locker;
}

void FTMBSharedState::_read_vor() {
    //TODO: check if locking is required or not
    std::memcpy(CAST_AWAY_PACKET_DATA(_vor_pkt) + DEFAULT_OFFSET, _maxs, sizeof(VectorOfClocks));
}

void FTMBSharedState::_transfer(int16_t queue, Packet* p, const Element::Port* output_port) {
    /// PAL packet
//    auto pal_pkt = _pal_pkt->clone();
    // Zero copy packet creation
    if (_pal_generated) {
        auto pal_pkt = Packet::make(CAST_AWAY_PACKET_DATA(_pal_pkt), _pal_pkt->length(), Util::no_op_pkt_destructor);
        std::memcpy(CAST_AWAY_PACKET_DATA(pal_pkt) + DEFAULT_OFFSET, &_pals[queue], sizeof(PacketAccessLog));
        output_port->push(pal_pkt);

        _pal_generated = false;
    }//if

    if (p)
        output_port->push(p);

    /// VOR packet, the content is already read into _vor_pkt
//    auto vor_pkt = _vor_pkt->clone();
    // Zero copy packet creation
    if (_pal_generated && _maxs[queue] % _batch == 0) {
        auto vor_pkt = Packet::make(CAST_AWAY_PACKET_DATA(_vor_pkt), _vor_pkt->length(), Util::no_op_pkt_destructor);
        output_port->push(vor_pkt);
    }//if
}

void FTMBSharedState::postprocess(int16_t queue, Locker* locker, Packet* p, const Element::Port* output_port) {
    if (_pal_generated && _maxs[queue] % _batch == 0) {
        /// create vector of clocks
        _read_vor();
    }//if

    /// release the lock
    UNLOCK(locker);

    /// transfer vor and pals
    _transfer(queue, p, output_port);
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