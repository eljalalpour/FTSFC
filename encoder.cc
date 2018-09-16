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

    auto msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
    for (int i = 0; i < MAX_CHAIN_LEN; ++i) {
        (*msg2[i]) = _msg[i];
    }//for

    DEBUG("End Encoder");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Encoder)
