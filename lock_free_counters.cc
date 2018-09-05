#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "lock_free_counters.hh"

CLICK_DECLS

LockFreeCounters::LockFreeCounters () {
    for (int i = 0; i < DEFAULT_SIZE; ++i)
        counters[i] = 0;
};

LockFreeCounters::~LockFreeCounters() { };

Packet *LockFreeCounters::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LockFreeCounters)