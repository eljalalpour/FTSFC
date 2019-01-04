#include "shared_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "nf_lock_free_counter.hh"

CLICK_DECLS

NFLockFreeCounter::NFLockFreeCounter () { };

NFLockFreeCounter::~NFLockFreeCounter() { };

void NFLockFreeCounter::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (SharedArray *)(r->find("array"));
}

int NFLockFreeCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .complete() < 0)
        return -1;
    DEBUG("NFLockFreeCounter index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *NFLockFreeCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin NFLockFreeCounter with index %d:", _index);

    _shared_state->increment(_index);

    DEBUG("End NFLockFreeCounter %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFLockFreeCounter)