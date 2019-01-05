#include <click/config.h>
#include "shared_state_base.hh"
#include <click/handlercall.hh>
#include <click/args.hh>

CLICK_DECLS

SharedStateBase::SharedStateBase () {
    for (int i = 0; i < STATE_LEN; ++i)
        _array[i] = 0;
};

SharedStateBase::~SharedStateBase() { };

void SharedStateBase::_init_shared_locks() {
    Router *r = this->router();
    _shared_locks = (SharedLocks *)(r->find(_shared_element_name));
}

Packet *SharedStateBase::simple_action(Packet *p) {
    return p;
}

int SharedStateBase::configure(Vector<String> &conf, ErrorHandler *errh) {
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

String SharedStateBase::read_handler(Element *e, void *) {
    SharedStateBase *c = (SharedStateBase *)e;
    String res;

    for (int i : c->_array) {
        res += String(i);
        res += ",";
    }//for

    return res;
}

void SharedStateBase::add_handlers() {
    add_read_handler("array", read_handler, LFA_H_ARRAY);
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedLocks)
EXPORT_ELEMENT(SharedStateBase)