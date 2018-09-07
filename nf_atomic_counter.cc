#include "atomic_array.hh"
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
    LOG("NFLockFreeCounter index is %d!\n", _index);

    return 0;
}

Packet *NFAtomicCounter::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin NFAtomicCounter with index %d:", _index);
    Router *r = this->router();

    AtomicArray *afc = (AtomicArray *)(r->find("array"));
    ++afc->counters[_index];

    LOG("End NFAtomicCounter %d:", _index);
    LOG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFAtomicCounter)
