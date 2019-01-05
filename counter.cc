#include "shared_state.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "counter.hh"

CLICK_DECLS

NFCounter::NFCounter () { };

NFCounter::~NFCounter() { };

void NFCounter::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (SharedState *)(r->find(_shared_state_element_name));
}

int NFCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("SHARED_STATE", _shared_state_element_name)
                .complete() < 0)
        return -1;

    DEBUG("NFCounter index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *NFCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin NFCounter with index %d:", _index);

    _shared_state->increment(_index);

    DEBUG("End NFCounter %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedState)
EXPORT_ELEMENT(NFCounter)
