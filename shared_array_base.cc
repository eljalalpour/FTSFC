#include <click/config.h>
#include "shared_array_base.hh"
#include <click/handlercall.hh>
#include <click/args.hh>

CLICK_DECLS

SharedArrayBase::SharedArrayBase () {
    for (int i = 0; i < DEFAULT_SIZE; ++i)
        _array[i] = 0;
};

SharedArrayBase::~SharedArrayBase() { };

void SharedArrayBase::_init_shared_locks() {
    Router *r = this->router();
    _shared_locks = (SharedLocks *)(r->find(_shared_element_name));
}

Packet *SharedArrayBase::simple_action(Packet *p) {
    return p;
}

int SharedArrayBase::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(this, errh).bind(conf)
                .read("SHARING_LEVEL", _sharing_level)
                .read("SHARED_LOCKS", _shared_element_name)
                .consume() < 0)
        return -1;

    assert(_sharing_level == ThreadSharing1 ||
           _sharing_level == ThreadSharing2 ||
           _sharing_level == ThreadSharing4 ||
           _sharing_level == ThreadSharing8);

    _init_shared_locks();

    return 0;
}

enum { LFA_H_ARRAY };

String SharedArrayBase::read_handler(Element *e, void *) {
    SharedArrayBase *c = (SharedArrayBase *)e;
    String res;

    for (int i : c->_array) {
        res += String(i);
        res += ",";
    }//for

    return res;
}

void SharedArrayBase::add_handlers() {
    add_read_handler("array", read_handler, LFA_H_ARRAY);
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedLocks)
EXPORT_ELEMENT(SharedArrayBase)