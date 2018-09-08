#include <click/config.h>
#include <click/router.hh>
#include "encoder.hh"

CLICK_DECLS

Encoder::Encoder () {
    // TODO: concurrency control on initializing _msg
    // Initialize message
    _util.random_message(_msg);
};

Encoder::~Encoder() { };

Packet *Encoder::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Encoder");

    // TODO: concurrency control on reading _msg
    _util.print(_msg);
    _util.encode(_msg, p);

    DEBUG("End Encoder");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Encoder)
