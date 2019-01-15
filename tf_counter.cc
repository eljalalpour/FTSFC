#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "tf_counter.hh"

CLICK_DECLS

TFCounter::TFCounter () : _counter(0) { };

TFCounter::~TFCounter() { };

void TFCounter::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (TFSharedStateCounter *)(r->find(_shared_state_element_name));
}

int TFCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("SHARED_STATE", _shared_state_element_name)
                .complete() < 0)
        return -1;

    DEBUG("TFCounter index is %d!\n", _index);

    _init_shared_state();

    return 0;
}

Packet *TFCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin TFCounter with index %d:", _index);

    Locker* locker = _shared_state->preprocess(_index, _index);

    _counter ++;

    _shared_state->postprocess(_index, locker, p, &output(0));

    DEBUG("End TFCounter %d:", _index);
    DEBUG("--------------------");

//    return p;
    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TFCounter)