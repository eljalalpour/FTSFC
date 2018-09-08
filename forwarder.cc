#include <click/config.h>
#include <click/router.hh>
#include "forwarder.hh"

CLICK_DECLS

Forwarder::Forwarder () {
    // TODO: concurrency control on initializing _msg
    // Initialize message
    _util.init(_msg);
};

Forwarder::~Forwarder() { };

Packet *Forwarder::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Forwarder");

    // TODO: concurrency control on reading _msg
    _util.encode(_msg, p);

    DEBUG("End Forwarder");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Forwarder)
