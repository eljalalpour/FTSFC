#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ftmb_counter.hh"

CLICK_DECLS

FTMBCounter::FTMBCounter () : _counter(0) { };

FTMBCounter::~FTMBCounter() { };

void FTMBCounter::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (FTMBSharedStateCounter *)(r->find(_shared_state_element_name));
}

int FTMBCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("SHARED_STATE", _shared_state_element_name)
                .complete() < 0)
        return -1;

    DEBUG("FTMBCounter index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *FTMBCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMBCounter with index %d:", _index);

    Locker* locker = _shared_state->preprocess(_index, _index);
    _counter ++;
    _shared_state->postprocess(_index, locker, p, &output(0));

    DEBUG("End FTMBCounter %d:", _index);
    DEBUG("--------------------");

//    return p;
    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBCounter)