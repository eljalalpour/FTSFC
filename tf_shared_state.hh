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
    AccessSharedVariable,
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

    virtual inline Locker* get_locker(int16_t, int16_t, TFOperation);

};

Locker* TFSharedState::get_locker(int16_t var_id, int16_t queue, TFOperation op) {
    Locker* locker = nullptr;

    switch (op) {
        case TFOperation::AccessSharedVariable:
            locker = _shared_locks->locker_ptr(var_id);
            break;
    }//switch

    return locker;
}