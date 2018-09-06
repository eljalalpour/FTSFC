#include "atomic_array.hh"

CLICK_DECLS

AtomicArray::AtomicArray () { };

AtomicArray::~AtomicArray() { };

Packet *AtomicArray::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AtomicArray)
