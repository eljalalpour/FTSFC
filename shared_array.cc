#include <click/config.h>
#include "shared_array.hh"
#include <click/handlercall.hh>

CLICK_DECLS

SharedArray::SharedArray () {
    for (int i = 0; i < DEFAULT_SIZE; ++i)
        array[i] = 0;
};

SharedArray::~SharedArray() { };

Packet *SharedArray::simple_action(Packet *p) {
    return p;
}

enum { LFA_H_ARRAY };

String SharedArray::read_handler(Element *e, void *) {
    SharedArray *c = (SharedArray *)e;
    String res;

    for(int i = 0; i < DEFAULT_HANDLER_SIZE; ++i) {
        res += String(c->array[i]);
        res += ",";
    }//for

    return res;
}

void SharedArray::add_handlers() {
    add_read_handler("array", read_handler, LFA_H_ARRAY);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LockFreeArray)