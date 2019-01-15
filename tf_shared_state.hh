#pragma once

#include <deque>
#include <cstring>
#include <click/packet_anno.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <clicknet/udp.h>
#include "shared_state_base.hh"
#include "transmitter.hh"

using std::deque;

#define COARSE_GRAINED_VOR_LOCK
#define FINE_GRAINED_VOR_LOCK

#define PAD          0
#define VOR_PKT_SIZE DEFAULT_OFFSET + sizeof(VectorOfClocks) + PAD
#define PAL_PKT_SIZE DEFAULT_OFFSET + sizeof(PacketAccessLog) + PAD

enum TFOperation {
    TFAccessSharedVariable,
};

class TFSharedState: public SharedStateBase {
public:
    TFSharedState ();

    ~TFSharedState();

    const char *class_name() const { return "TFSharedState"; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    int initialize(ErrorHandler *);

    inline Locker* preprocess(int16_t, int16_t);

    inline void postprocess(int16_t, Locker*, Packet*, const Element::Port*);

protected:
    int _queues;
    int _batch;
    String _trans_element_name;
    Transmitter* _trans;

    vector<Packet*> _queued_packets[MAX_QUEUES];
    int _queued_packets_count[MAX_QUEUES];

    virtual inline Locker* get_locker(int16_t, int16_t, TFOperation = TFOperation::TFAccessSharedVariable);
};

Locker* TFSharedState::get_locker(int16_t var_id, int16_t queue, TFOperation op) {
    Locker* locker = nullptr;

    switch (op) {
        case TFOperation::TFAccessSharedVariable:
            locker = _shared_locks->locker_ptr(var_id);
            break;
    }//switch

    return locker;
}

Locker* TFSharedState::preprocess(int16_t var_id, int16_t queue) {
    return get_locker(var_id, queue);
}

void TFSharedState::postprocess(int16_t queue, Locker* locker, Packet* p, const Element::Port* output_port) {
    _queued_packets[queue][_queued_packets_count[queue]++] = p;
    UNLOCK(locker);

    if (_queued_packets_count[queue] < _batch) {
        return;
    }//if

    _trans->send(queue);

    for (int i = 0; i < _queued_packets_count[queue]; ++i) {
        output_port->push(_queued_packets[queue][i]);
        _queued_packets[queue][i] = 0;
    }//for

    _queued_packets_count[queue] = 0;
}
