#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <mutex>
#include <click/hashcontainer.hh>
#include "ft_shared_state.hh"

CLICK_DECLS

class FTSharedStateAddrRewriter : public FTSharedState {
public:
    FTSharedStateAddrRewriter ();

    ~FTSharedStateAddrRewriter();

    const char *class_name() const { return "FTSharedStateAddrRewriter"; }
};

CLICK_ENDDECLS