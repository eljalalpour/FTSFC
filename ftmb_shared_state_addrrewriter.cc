#include <click/config.h>
#include "ftmb_shared_state_addrrewriter.hh"
#include <click/handlercall.hh>
#include <click/args.hh>
#include <cstring>

CLICK_DECLS

FTMBSharedStateAddrRewriter::FTMBSharedStateAddrRewriter() {
    std::memset(_pals, 0, sizeof(_pals));
}

FTMBSharedStateAddrRewriter::~FTMBSharedStateAddrRewriter() { }

int FTMBSharedStateAddrRewriter::configure(Vector<String> &conf, ErrorHandler *errh) {
    return FTMBSharedState::configure(conf, errh);
}

Packet *FTMBSharedStateAddrRewriter::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(FTMBSharedState)
EXPORT_ELEMENT(FTMBSharedStateAddrRewriter)
