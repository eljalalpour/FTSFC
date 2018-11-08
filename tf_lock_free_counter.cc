#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "tf_lock_free_counter.hh"

CLICK_DECLS

TFLockFreeCounter::TFLockFreeCounter  () : _trans_init(false) { };

TFLockFreeCounter::~TFLockFreeCounter () { };

int TFLockFreeCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("BATCH", _batch)
                .read("INDEX", _index)
                .complete() < 0)
        return -1;

    LOG("TFLockFreeCounter index is %d, batch is %d!\n", _index, _batch);

    for (int i = 0; i < _batch; ++i) {
        _queue.push_back(0);
    }//for
    _queued_packets = 0;

    return 0;
}

void TFLockFreeCounter::_init_transmitter() {
    if (!_trans_init) {
        Router *r = this->router();
        _trans = (Transmitter *)(r->find("trans"));

        _trans_init = true;
    }//if
}

void TFLockFreeCounter::push(int, Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin TFLockFreeCounter");

    _init_transmitter();
    _queue[_queued_packets++] = p;
    (*_trans).inoperation[_index]++;

    if (_queued_packets < _batch)
        return;

    _trans->send();

    for (int i = 0; i < _queued_packets; ++i) {
        output(0).push(_queue[i]);
        _queue[i] = 0;
    }//for

    _queued_packets = 0;

    DEBUG("End TFLockFreeCounter");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFLockFreeCounter)