#include <click/config.h>
#include "atomic_array.hh"
#include <click/handlercall.hh>

CLICK_DECLS

AtomicArray::AtomicArray () { };

AtomicArray::~AtomicArray() { };

Packet *AtomicArray::simple_action(Packet *p) {
    return p;
}

enum { H_COUNT };

String LockFreeArray::read_handler(Element *e, void *thunk) {
    LockFreeArray *c = (LockFreeArray *)e;
    String res;

    for(int i = 0; i < DEFAULT_HANDLER_SIZE; ++i) {
        res += String(c->counters[i]);
        res += ",";
    }//for

    return res;
}

void LockFreeArray::add_handlers() {
    add_read_handler("count", read_handler, H_COUNT);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AtomicArray)
