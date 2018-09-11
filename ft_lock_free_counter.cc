#include "shared_lock_free_state.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ft_lock_free_counter.hh"

CLICK_DECLS

FTLockFreeCounter::FTLockFreeCounter () { };

FTLockFreeCounter::~FTLockFreeCounter() { };

int FTLockFreeCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .complete() < 0)
        return -1;
    DEBUG("FTLockFreeCounter index is %d!\n", _index);

    return 0;
}

Packet *FTLockFreeCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTLockFreeCounter with index %d:", _index);
    Router *r = this->router();

    SharedLockFreeState *lfs = (SharedLockFreeState *)(r->find("shared_state"));
    ++lfs->in_operation[_index];

    DEBUG("End FTLockFreeCounter %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTLockFreeCounter)
