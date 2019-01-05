#include "shared_state.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "counter.hh"

CLICK_DECLS

Counter::Counter () { };

Counter::~Counter() { };

void Counter::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (SharedState *)(r->find(_shared_state_element_name));
}

int Counter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("SHARED_STATE", _shared_state_element_name)
                .complete() < 0)
        return -1;

    DEBUG("Counter index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *Counter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Counter with index %d:", _index);

    _shared_state->increment(_index);

    DEBUG("End Counter %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(SharedState)
EXPORT_ELEMENT(Counter)
