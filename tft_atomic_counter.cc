#include "atomic_counters.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "Transmitter.hh"
#include "tft_atomic_counter.hh"

CLICK_DECLS

TFTAtomicCounter::TFTAtomicCounter  () : _index(DEFAULT_INDEX) { };

TFTAtomicCounter ::~TFTAtomicCounter () { };

int TFTAtomicCounter ::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    Args(conf, this, errh).read_or_set("INDEX", _index, DEFAULT_INDEX);
    LOG("TFTLockFreeCounter index is %d!\n", _index);
    _key = std::to_string(_index);
    _val = DEFAULT_VALUE;

    return 0;
}

Packet *TFTAtomicCounter ::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin TFTLockFreeCounter  with index %d:", _index);
    Router *r = this->router();

    AtomicCounters *lfc = (AtomicCounters *)(r->find("counters"));
    Transmitter *trans = (Transmitter *)(r->find("trans"));

    ++lfc->counters[_index];
    FTState state;
    state[_key] = _val;
    trans->send(state);

    LOG("End TFTLockFreeCounter %d:", _index);
    LOG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFTLockFreeCounter )
