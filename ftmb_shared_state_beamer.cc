#include <click/config.h>
#include "ftmb_shared_state_beamer.hh"
#include <click/handlercall.hh>
#include <click/args.hh>
#include <cstring>

CLICK_DECLS

FTMBSharedStateBeamer::FTMBSharedStateBeamer() {
    std::memset(_pals, 0, sizeof(_pals));
}

FTMBSharedStateBeamer::~FTMBSharedStateBeamer() { }

int FTMBSharedStateBeamer::configure(Vector<String> &conf, ErrorHandler *errh) {
    return FTMBSharedState::configure(conf, errh);
}

Packet *FTMBSharedStateBeamer::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(FTSharedState)
EXPORT_ELEMENT(FTMBSharedStateBeamer)
