#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "tf_lock_free_counter.hh"

CLICK_DECLS

TFLockFreeCounter::TFLockFreeCounter  () : _index(DEFAULT_INDEX) { };

TFLockFreeCounter::~TFLockFreeCounter () { };

int TFLockFreeCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    Args(conf, this, errh).read_or_set("INDEX", _index, DEFAULT_INDEX);
    DEBUG("TFLockFreeCounter index is %d!\n", _index);

    return 0;
}

void TFLockFreeCounter::_init_transmitter() {
    if (!_trans_init) {
        Router *r = this->router();
        _trans = (Transmitter *)(r->find("trans"));

        _init = true;
    }//if
}

Packet *TFLockFreeCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin TFLockFreeCounter");

    _init_transmitter();

    ++ _trans->inoperation[_index];

    _trans->send();

    DEBUG("End TFLockFreeCounter");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFLockFreeCounter)
