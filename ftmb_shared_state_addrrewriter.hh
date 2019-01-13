#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include "ftmb_shared_state.hh"

CLICK_DECLS

class FTMBSharedStateAddrRewriter : public FTMBSharedState {
public:
    FTMBSharedStateAddrRewriter ();

    ~FTMBSharedStateAddrRewriter();

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    const char *class_name() const { return "FTMBSharedStateAddrRewriter"; }
};

CLICK_ENDDECLS
