#include "atomic_counters.hh"

CLICK_DECLS

AtomicCounters::AtomicCounters () { };

AtomicCounters::~AtomicCounters() { };

Packet *AtomicCounters::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AtomicCounters)
