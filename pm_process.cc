#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "pm_process.hh"

CLICK_DECLS

PMProcess::PMProcess() {
    Util _util;
    _util.init(_log_table);
}

PMProcess::~PMProcess() { }

inline void PMProcess::_init_shared_state_pointer() {
    Router *r = this->router();
    _shared_state = (FTSharedState *)(r->find("shared_state"));
}

Packet* PMProcess::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin PMProcess");

    try {
        _shared_state->process_piggyback_message(p, _log_table, _queue);
    }//try
    catch(...) {
        p->kill();
        return 0;
    }//catch

    DEBUG("End PMProcess");
    DEBUG("--------------------");

    return p;
}

int PMProcess::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("QUEUE", _queue)
                .complete() < 0)
        return -1;

    LOG("PMProcess queue is %d!\n", _queue);

    _init_shared_state_pointer();

    return 0;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(PMProcess)
