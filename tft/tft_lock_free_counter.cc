#include "../lock_free_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "transmitter.hh"
#include "tft_lock_free_counter.hh"

CLICK_DECLS

TFTLockFreeCounter::TFTLockFreeCounter  () : _index(DEFAULT_INDEX) { };

TFTLockFreeCounter ::~TFTLockFreeCounter () { };

int TFTLockFreeCounter ::configure(Vector<String> &conf, ErrorHandler *errh) {
    //TODO
//    // set index param
//    Args(conf, this, errh).read_or_set("INDEX", _index, DEFAULT_INDEX);
//    DEBUG("TFTLockFreeCounter index is %d!\n", _index);
//    _key = std::to_string(_index);
//    _val = DEFAULT_VALUE;

    return 0;
}

Packet *TFTLockFreeCounter ::simple_action(Packet *p) {
    //TODO
//    DEBUG("--------------------");
//    DEBUG("Begin TFTLockFreeCounter  with index %d:", _index);
//    Router *r = this->router();
//
//    LockFreeArray *lfc = (LockFreeArray *)(r->find("array"));
//    Transmitter *trans = (Transmitter *)(r->find("trans"));
//
//    ++lfc->counters[_index];
//    FTState state;
//    state[_key] = _val;
//    trans->send(state);
//
//    DEBUG("End TFTLockFreeCounter %d:", _index);
//    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFTLockFreeCounter )
