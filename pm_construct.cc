#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "pm_construct.hh"

CLICK_DECLS

PMConstruct::PMConstruct() { }

PMConstruct::~PMConstruct() { }

inline void PMConstruct::_init_shared_state_pointer() {
    Router *r = this->router();
    _shared_state = (SharedState *)(r->find("shared_state"));
}

int PMConstruct::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("QUEUE", _queue)
                .complete() < 0)
        return -1;

    LOG("PMConstruct queue is %d!\n", _queue);

    _init_shared_state_pointer();

    return 0;
}

Packet *PMConstruct::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin PMConstruct");

    _shared_state->construct_piggyback_message(p, _queue);

    DEBUG("End PMConstruct");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PMConstruct)
