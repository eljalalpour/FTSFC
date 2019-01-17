#include <click/config.h>
#include "tf_shared_state_counter.hh"
#include <click/handlercall.hh>
#include <click/args.hh>

CLICK_DECLS

TFSharedStateCounter::TFSharedStateCounter() { }

TFSharedStateCounter::~TFSharedStateCounter() { }

int TFSharedStateCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(this, errh).bind(conf)
                .read("SHARING_LEVEL", _sharing_level)
                .consume() < 0)
        return -1;

    assert(_sharing_level == ThreadSharing1 ||
           _sharing_level == ThreadSharing2 ||
           _sharing_level == ThreadSharing4 ||
           _sharing_level == ThreadSharing8);

    LOG("FTSharedStateCounter, sharing level: %d", _sharing_level);

    auto re = TFSharedState::configure(conf, errh);
    return re;
}

Packet *TFSharedStateCounter::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(TFSharedState)
EXPORT_ELEMENT(TFSharedStateCounter)
