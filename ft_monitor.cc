#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ft_monitor.hh"

CLICK_DECLS

FTMonitor::FTMonitor () : _init_state(false) { };

FTMonitor::~FTMonitor() { };

void FTMonitor::_init_shared_state() {
    if (!_init_state) {
        Router *r = this->router();
        _shared_state = (SharedLockFreeState *)(r->find("shared_state"));
        _init_state = true;
    }//if
}

int FTMonitor::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("FOR", _for_count)
                .complete() < 0)
        return -1;

    DEBUG("FTMonitor index is %d!\n", _index);

    return 0;
}

Packet *FTMonitor::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMonitor with index %d:", _index);

    _init_shared_state();

    ++_shared_state->inoperation[_index];

    for (int i = 0; i < _for_count; ++i) {
        rand();
    }//for

    DEBUG("End FTMonitor %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMonitor)
