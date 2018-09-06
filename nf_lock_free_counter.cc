#include "lock_free_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "nf_lock_free_counter.hh"

CLICK_DECLS

NFLockFreeCounter::NFLockFreeCounter () : _index(DEFAULT_INDEX) { };

NFLockFreeCounter::~NFLockFreeCounter() { };

int NFLockFreeCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    Args(conf, this, errh).read_or_set("INDEX", _index, DEFAULT_INDEX);
    LOG("NFLockFreeCounter index is %d!\n", _index);

    return 0;
}

Packet *NFLockFreeCounter::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin NFLockFreeCounter with index %d:", _index);
    Router *r = this->router();

    LockFreeArray *lfc = (LockFreeArray *)(r->find("array"));
    ++lfc->counters[_index];

    LOG("End NFLockFreeCounter %d:", _index);
    LOG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFLockFreeCounter)
