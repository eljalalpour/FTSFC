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
    size_t _readers, _writers;
    if (Args(conf, this, errh)
                .read("READERS", _readers)
                .read("WRITERS", _writers)
                .complete() < 0)
        return -1;

    assert(_readers > 0);
    assert(_writers > 0);

    extend_readers_size(_readers);
    extend_writers_size(_writers);

    return 0;
}

Packet *SharedLocks::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SharedLocks)
