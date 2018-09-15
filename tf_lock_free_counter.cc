#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "tf_lock_free_counter.hh"

CLICK_DECLS

TFLockFreeCounter::TFLockFreeCounter  () {

};

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

Packet *TFLockFreeCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin TFLockFreeCounter");

    _init_transmitter();
    LOG("After init");

    ++ _trans->inoperation[_index];
    LOG("After increment");

    _trans->send();
    LOG("After send");

    DEBUG("End TFLockFreeCounter");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFLockFreeCounter)
