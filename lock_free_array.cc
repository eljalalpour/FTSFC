#pragma once

#include <click/config.h>
#include <click/element.hh>
#include "lock_free_array.hh"

CLICK_DECLS

LockFreeArray::LockFreeArray () {
    for (int i = 0; i < DEFAULT_SIZE; ++i)
        counters[i] = 0;
};

LockFreeArray::~LockFreeArray() { };

Packet *LockFreeArray::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LockFreeArray)