#include <click/config.h>
#include "ftmb_shared_state_counter.hh"
#include <click/handlercall.hh>
#include <click/args.hh>
#include <cstring>

CLICK_DECLS

FTMBSharedStateCounter::FTMBSharedStateCounter() {
    std::memset(_pals, 0, sizeof(_pals));
}

FTMBSharedStateCounter::~FTMBSharedStateCounter() { }

int FTMBSharedStateCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(this, errh).bind(conf)
                .read("SHARING_LEVEL", _sharing_level)
                .consume() < 0)
        return -1;

    assert(_sharing_level == ThreadSharing1 ||
           _sharing_level == ThreadSharing2 ||
           _sharing_level == ThreadSharing4 ||
           _sharing_level == ThreadSharing8);

    LOG("FTMBSharedStateCounter sharing level: %d", _sharing_level);

    return FTMBSharedState::configure(conf, errh);
}

Packet *FTMBSharedStateCounter::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(FTMBSharedState)
EXPORT_ELEMENT(FTMBSharedStateCounter)
