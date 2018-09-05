#include "atomic_counters.hh"

CLICK_DECLS

Packet *AtomicCounters::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AtomicCounters)
