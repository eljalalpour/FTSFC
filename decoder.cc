#include <click/config.h>
#include <click/router.hh>
#include "decoder.hh"

CLICK_DECLS

Decoder::Decoder () { };

Decoder::~Decoder() { };

Packet *Decoder::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Decoder");

    _util.decode(_msg, p);

    DEBUG("End Decoder");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Decoder)
