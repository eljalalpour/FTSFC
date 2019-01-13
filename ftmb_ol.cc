#include "shared_state.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ftmb_ol.hh"
#include "ftmb_shared_state.hh"

CLICK_DECLS

FTMBOutputLogger::FTMBOutputLogger () { };

FTMBOutputLogger::~FTMBOutputLogger() { };

Packet *FTMBOutputLogger::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMBOutputLogger");

//    if (p->length() == PAL_PKT_SIZE) {
//        _process_pal(p);
//        p->kill();
//        return 0;
//    }//if
//    else if (p->length() == VOR_PKT_SIZE) {
//        _process_vor(p);
//        p->kill();
//        return 0;
//    }//else if

    if (p->length() == EXTRA_PKT_SIZE) {
        _process_extra(p);
    }//if

    DEBUG("End FTMBOutputLogger");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBOutputLogger)
