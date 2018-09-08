#include <click/config.h>
#include "atomic_array.hh"
#include <click/handlercall.hh>

CLICK_DECLS

AtomicArray::AtomicArray () {
    for (int i = 0; i < DEFAULT_SIZE; ++i)
        counters[i] = 0;
};

AtomicArray::~AtomicArray() { };

Packet *AtomicArray::simple_action(Packet *p) {
    return p;
}

enum { ATA_H_COUNT };

String AtomicArray::read_handler(Element *e, void *) {
    AtomicArray *c = (AtomicArray *)e;
    String res;

    for(int i = 0; i < DEFAULT_HANDLER_SIZE; ++i) {
        res += String(c->counters[i]);
        res += ",";
    }//for

    return res;
}

void AtomicArray::add_handlers() {
    add_read_handler("count", read_handler, ATA_H_COUNT);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AtomicArray)
