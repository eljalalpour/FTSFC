#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ft_monitor.hh"

CLICK_DECLS

FTMonitor::FTMonitor () { };

FTMonitor::~FTMonitor() { };

void FTMonitor::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (FTSharedState *)(r->find("shared_state"));
}

int FTMonitor::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("FOR", _for_count)
                .complete() < 0)
        return -1;

    DEBUG("FTMonitor index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *FTMonitor::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMonitor with index %d:", _index);

//    ++_shared_state->_inoperation[_index];
    _shared_state->increment(_index);

    for (int i = 0; i < _for_count; ++i) {
        rand();
    }//for

    DEBUG("End FTMonitor %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMonitor)
