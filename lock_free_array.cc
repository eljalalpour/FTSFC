#include <click/config.h>
#include "lock_free_array.hh"
#include <click/handlercall.hh>

CLICK_DECLS

LockFreeArray::LockFreeArray () {
    for (int i = 0; i < DEFAULT_SIZE; ++i)
        array[i] = 0;
};

LockFreeArray::~LockFreeArray() { };

Packet *LockFreeArray::simple_action(Packet *p) {
    return p;
}

enum { LFA_H_ARRAY };

String LockFreeArray::read_handler(Element *e, void *) {
    LockFreeArray *c = (LockFreeArray *)e;
    String res;

    for(int i = 0; i < DEFAULT_HANDLER_SIZE; ++i) {
        res += String(c->array[i]);
        res += ",";
    }//for

    return res;
}

void LockFreeArray::add_handlers() {
    add_read_handler("array", read_handler, LFA_H_ARRAY);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LockFreeArray)