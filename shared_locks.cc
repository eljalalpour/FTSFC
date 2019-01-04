#include <click/config.h>
#include <click/args.hh>
#include <algorithm>
#include <cstring>
#include "shared_locks.hh"

CLICK_DECLS

SharedLocks::SharedLocks() { };

SharedLocks::~SharedLocks() { };

int SharedLocks::configure(Vector <String> &conf, ErrorHandler *errh) {
    // set id and f params
    size_t _locks;
    if (Args(conf, this, errh)
                .read("LOCKS", _locks)
                .complete() < 0)
        return -1;

    extend(_locks);

    return 0;
}

Packet *SharedLocks::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLocks)
