#include "atomic_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "transmitter.hh"
#include "tft_atomic_counter.hh"

CLICK_DECLS

TFTAtomicCounter::TFTAtomicCounter  () : _index(DEFAULT_INDEX) { };

TFTAtomicCounter::~TFTAtomicCounter () { };

int TFTAtomicCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    //TODO
//    // set index param
//    Args(conf, this, errh).read_or_set("INDEX", _index, DEFAULT_INDEX);
//    DEBUG("TFTAtomicCounter index is %d!\n", _index);
//    _key = std::to_string(_index);
//    _val = DEFAULT_VALUE;

    return 0;
}

Packet *TFTAtomicCounter::simple_action(Packet *p) {
    //TODO
//    DEBUG("--------------------");
//    DEBUG("Begin TFTAtomicCounter with index %d:", _index);
//    Router *r = this->router();
//
//    AtomicArray *lfc = (AtomicArray *)(r->find("array"));
//    Transmitter *trans = (Transmitter *)(r->find("trans"));
//
//    ++lfc->counters[_index];
//    FTState state;
//    state[_key] = _val;
//    trans->send(state);
//
//    DEBUG("End TFTAtomicCounter %d:", _index);
//    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFTAtomicCounter)
