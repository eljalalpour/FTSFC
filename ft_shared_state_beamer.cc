#include <click/config.h>
#include "ft_shared_state_beamer.hh"
#include <click/handlercall.hh>
#include <click/args.hh>

CLICK_DECLS

FTSharedStateBeamer::FTSharedStateBeamer() { }

FTSharedStateBeamer::~FTSharedStateBeamer() { }

int FTSharedStateBeamer::configure(Vector<String> &conf, ErrorHandler *errh) {
    return FTSharedState::configure(conf, errh);
}

Packet *FTSharedStateBeamer::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(FTSharedState)
EXPORT_ELEMENT(FTSharedStateBeamer)
