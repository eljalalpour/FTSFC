#include <click/config.h>
#include <click/router.hh>
#include "encoder.hh"

CLICK_DECLS

Encoder::Encoder () {
    // TODO: concurrency control on initializing _msg
    // Initialize message
//    _util.random_message(_msg);
};

Encoder::~Encoder() { };

Packet *Encoder::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Encoder");

    // TODO: concurrency control on reading _msg
    _util.random_message(_msg);

    DEBUG("--------------------");
    DEBUG("Before:");
    _util.print(_msg);
    _util.encode(_msg, p);

    PiggyBackMessage after;
    _util.decode(after, p);

    DEBUG("--------------------");
    DEBUG("After:");
    _util.print(after);

    DEBUG("End Encoder");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Encoder)
