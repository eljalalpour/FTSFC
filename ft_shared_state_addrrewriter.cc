#include <click/config.h>
#include "ft_shared_state_addrrewriter.hh"
#include <click/handlercall.hh>
#include <click/args.hh>

CLICK_DECLS

FTSharedStateAddrRewriter::FTSharedStateAddrRewriter() { }

FTSharedStateAddrRewriter::~FTSharedStateAddrRewriter() { }

int FTSharedStateAddrRewriter::configure(Vector<String> &conf, ErrorHandler *errh) {
    return FTSharedState::configure(conf, errh);
}

Packet *FTSharedStateAddrRewriter::simple_action(Packet *p) {
    return p;
}

void FTSharedStateAddrRewriter::register_addr_rewriter(int queue, FTAddrRewriter* me) {
    _reg_addr_rw[queue] = me;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(FTSharedState)
EXPORT_ELEMENT(FTSharedStateAddrRewriter)
