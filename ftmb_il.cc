#include "shared_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ftmb_il.hh"

CLICK_DECLS

FTMBInputLogger::FTMBInputLogger () { };

FTMBInputLogger::~FTMBInputLogger() { };

Packet *FTMBInputLogger::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBInputLogger)
