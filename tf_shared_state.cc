#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include <cstring>
#include "tf_shared_state.hh"
#include <click/glue.hh>
#include <click/standard/alignmentinfo.hh>
#include <click/etheraddress.hh>
#include <click/error.hh>

CLICK_DECLS

TFSharedState::TFSharedState() : _pal_generated(false) { };

TFSharedState::~TFSharedState() { };

int TFSharedState::configure(Vector <String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("QUEUES", _queues)
                .read("BATCH", _batch)
                .read("TRANSMITTER", _trans_element_name)
                .consume() < 0)
        return -1;

    Router *r = this->router();
    _trans = (Transmitter *)(r->find(_trans_element_name));

    LOG("TF Shared state, queues: %d, batch: %d", _queues, _batch);

    for (int i = 0; i < _queues; ++i) {
        for (int j = 0; j < _batch; ++j) {
            _queued_packets[i].push_back(0);
        }//for
        _queued_packets_count[i] = 0;
    }//for

    return SharedStateBase::configure(conf, errh);
}

int TFSharedState::initialize(ErrorHandler *) {
    return 0;
}

Locker* TFSharedState::preprocess(int16_t var_id, int16_t queue) {
    return get_locker(var_id, queue);
}

void TFSharedState::postprocess((int16_t queue, Locker* locker, Packet* p, const Element::Port* output_port)) {
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

Packet *TFSharedState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedStateBase)
EXPORT_ELEMENT(TFSharedState)
