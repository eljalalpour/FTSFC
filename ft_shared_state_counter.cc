#include <click/config.h>
#include "ft_shared_state_counter.hh"
#include <click/handlercall.hh>
#include <click/args.hh>

CLICK_DECLS

int FTSharedStateCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(this, errh).bind(conf)
                .read("SHARING_LEVEL", _sharing_level)
                .consume() < 0)
        return -1;

    assert(_sharing_level == ThreadSharing1 ||
           _sharing_level == ThreadSharing2 ||
           _sharing_level == ThreadSharing4 ||
           _sharing_level == ThreadSharing8);

    return FTSharedState::configure(conf, errh);
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(FTSharedState)
EXPORT_ELEMENT(FTSharedStateCounter)
