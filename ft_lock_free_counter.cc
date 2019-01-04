#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ft_lock_free_counter.hh"

CLICK_DECLS

FTLockFreeCounter::FTLockFreeCounter () { };

FTLockFreeCounter::~FTLockFreeCounter() { };

void FTLockFreeCounter::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (SharedLockFreeState *)(r->find("shared_state"));
}

int FTLockFreeCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .complete() < 0)
        return -1;

    DEBUG("FTLockFreeCounter index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *FTLockFreeCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTLockFreeCounter with index %d:", _index);

//    ++_shared_state->_inoperation[_index];
    _shared_state->increment(_index);

    DEBUG("End FTLockFreeCounter %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTLockFreeCounter)
