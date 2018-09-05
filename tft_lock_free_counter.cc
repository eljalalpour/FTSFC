#include "lock_free_counters.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "tft_lock_free_counter.hh"

CLICK_DECLS

TFTLockFreeCounter::TFTLockFreeCounter  () : _index(DEFAULT_INDEX) { };

TFTLockFreeCounter ::~TFTLockFreeCounter () { };

int TFTLockFreeCounter ::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    Args(conf, this, errh).read_or_set("INDEX", _index, DEFAULT_INDEX);
    LOG("TFTLockFreeCounter index is %d!\n", _index);

    return 0;
}

Packet *TFTLockFreeCounter ::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin TFTLockFreeCounter  with index %d:", _index);
    Router *r = this->router();

    LockFreeCounters *lfc = (LockFreeCounters *)(r->find("counters"));
    ++lfc->counters[_index];

    LOG("End TFTLockFreeCounter %d:", _index);
    LOG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFTLockFreeCounter )
