#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "tf_lock_free_counter.hh"

CLICK_DECLS

TFLockFreeCounter::TFLockFreeCounter  () : _trans_init(false), _queued_packets(0) { };

TFLockFreeCounter::~TFLockFreeCounter () { };

int TFLockFreeCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    Args(conf, this, errh).read_or_set("INDEX", _index, DEFAULT_INDEX);
    LOG("TFLockFreeCounter index is %d!\n", _index);

    return 0;
}

void TFLockFreeCounter::_init_transmitter() {
    if (!_trans_init) {
        Router *r = this->router();
        _trans = (Transmitter *)(r->find("trans"));

        _trans_init = true;
    }//if
}

void TFLockFreeCounter::push(int, Packet*p) {
    DEBUG("--------------------");
    DEBUG("Begin TFLockFreeCounter");

    _queue[_queued_packets++] = p;
    while (_queued_packets < QUEUE_LEN) {
        _queue[_queued_packets++] = input(0).pull();
    }//while

    _init_transmitter();

    for (int i = 0; i < _queued_packets; ++i) {
        (*_trans).inoperation[_index]++;
        _trans->send();
    }//for

    _trans->recv();

    for (int i = 0; i < _queued_packets; ++i) {
        output(0).push(_queue[i]);
        _queue[i] = 0;
    }//for

    _queued_packets = 0;

//    if (_queued_packets >= QUEUE_LEN) {
//        _init_transmitter();
//
//        for (int i = 0; i < _queued_packets; ++i) {
//            (*_trans).inoperation[_index]++;
//            _trans->send();
//        }//for
//
//        _trans->recv();
//
//        for (int i = 0; i < _queued_packets; ++i) {
//            output(0).push(_queue[i]);
//            _queue[i] = 0;
//        }//for
//
//        _queued_packets = 0;
//    }//else

//    LOG("Queued packets: %d!", _queued_packets);
//    _queue[_queued_packets++] = Packet::make(p->data(), p->length());
//    p->kill();

    DEBUG("End TFLockFreeCounter");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFLockFreeCounter)
