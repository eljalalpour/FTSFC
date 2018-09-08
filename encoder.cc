#include <click/config.h>
#include <click/router.hh>
#include "encoder.hh"

CLICK_DECLS

Encoder::Encoder () {
    _util.random_message(_msg);
};

Encoder::~Encoder() { };

Packet *Encoder::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Encoder");

    _util.encode(_msg, p);

    DEBUG("End Encoder");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Encoder)
