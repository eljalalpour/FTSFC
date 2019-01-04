#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "nf_monitor.hh"

CLICK_DECLS

NFMonitor::NFMonitor () { };

NFMonitor::~NFMonitor() { };

void NFMonitor::_init_shared_state() {
    Router *r = this->router();
    _lock_free_array = (SharedArray *)(r->find("array"));
}

int NFMonitor::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("FOR", _for_count)
                .complete() < 0)
        return -1;

    DEBUG("NFMonitor index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *NFMonitor::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin NFMonitor with index %d:", _index);

    ++(_lock_free_array->array[_index]);

    for (int i = 0; i < _for_count; ++i) {
        rand();
    }//for

    DEBUG("End NFMonitor %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFMonitor)
