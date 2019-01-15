#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include <cstring>
#include "tf_shared_state.hh"
#include <click/glue.hh>
#include <click/router.hh>
#include <click/standard/alignmentinfo.hh>
#include <click/etheraddress.hh>
#include <click/error.hh>

CLICK_DECLS

TFSharedState::TFSharedState() { };

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

Packet *TFSharedState::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedStateBase)
EXPORT_ELEMENT(TFSharedState)
