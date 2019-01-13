#include <click/config.h>
#include "ft_shared_state_addrrewriter.hh"
#include <click/handlercall.hh>
#include <click/args.hh>

CLICK_DECLS

FTSharedStateAddrRewriter::FTSharedStateAddrRewriter() { }

FTSharedStateAddrRewriter::~FTSharedStateAddrRewriter() { }

CLICK_ENDDECLS
ELEMENT_REQUIRES(FTSharedState)
EXPORT_ELEMENT(FTSharedStateAddrRewriter)
