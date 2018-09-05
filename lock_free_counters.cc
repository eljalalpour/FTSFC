#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "lock_free_counters.cc"

CLICK_DECLS

LockFreeCounters::LockFreeCounters () : counters {0} { };

LockFreeCounters::~LockFreeCounters() { };

Packet *LockFreeCounters::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LockFreeCounters)