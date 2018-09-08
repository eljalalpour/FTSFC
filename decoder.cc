#include <click/config.h>
#include <click/router.hh>
#include "decoder.hh"

CLICK_DECLS

Decoder::Decoder () {
    // TODO: concurrency control on initializing _msg
};

Decoder::~Decoder() { };

Packet *Decoder::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Decoder");

    // TODO: concurrency control on reading _msg
    _util.decode(_msg, p);
    _util.print(_msg);

    DEBUG("End Decoder");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Decoder)
