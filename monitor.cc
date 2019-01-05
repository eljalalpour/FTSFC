#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "monitor.hh"

CLICK_DECLS

Monitor::Monitor () { };

Monitor::~Monitor() { };

void Monitor::_init_shared_state() {
    Router *r = this->router();
    _lock_free_array = (SharedState *)(r->find("array"));
}

int Monitor::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("FOR", _for_count)
                .complete() < 0)
        return -1;

    DEBUG("Monitor index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *Monitor::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Monitor with index %d:", _index);

    _lock_free_array->increment(_index);

    for (int i = 0; i < _for_count; ++i) {
        rand();
    }//for

    DEBUG("End Monitor %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFMonitor)
