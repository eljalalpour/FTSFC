#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ft_counter.hh"

CLICK_DECLS

FTCounter::FTCounter () { };

FTCounter::~FTCounter() { };

void FTCounter::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (FTSharedStateCounter *)(r->find(_shared_state_element_name));
}

int FTCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("SHARED_STATE", _shared_state_element_name)
                .complete() < 0)
        return -1;

    DEBUG("FTCounter index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *FTCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTCounter with index %d:", _index);

    _shared_state->increment(_index);

    DEBUG("End FTCounter %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTCounter)
