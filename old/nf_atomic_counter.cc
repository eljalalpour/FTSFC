#include "../atomic_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "nf_atomic_counter.hh"

CLICK_DECLS

NFAtomicCounter::NFAtomicCounter () : _index(DEFAULT_INDEX) { };

NFAtomicCounter::~NFAtomicCounter() { };

int NFAtomicCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .complete() < 0)
        return -1;
    DEBUG("NFLockFreeCounter index is %d!\n", _index);

    return 0;
}

Packet *NFAtomicCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin NFAtomicCounter with index %d:", _index);
    Router *r = this->router();

    AtomicArray *afc = (AtomicArray *)(r->find("array"));
    ++afc->array[_index];

    DEBUG("End NFAtomicCounter %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFAtomicCounter)
