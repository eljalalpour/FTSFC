#include <click/config.h>
#include <click/router.hh>
#include "forwarder.hh"

CLICK_DECLS

Forwarder::Forwarder () {
    // Initialize message
    _util.init(_msg);
};

Forwarder::~Forwarder() { };

void Forwarder::push(int source, Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Forwarder");

    if (source == 0) { // Receiving a packet from the traffic source
        // Encode its memory of the piggyback message into the packet
        //TODO: make sure no lock is required for encoding and decoding
        _util.encode(_msg, p);

//        LOG("Forwarder Encode:");
//        _util.print(_msg[0]);
//        _util.print(_msg[1]);

        output(0).push(p);
    }//if
    else { //Receiving a packet from Buffer
        // Decode and memorize the piggyback message from the packet
        //TODO: make sure no lock is required for encoding and decoding
        PiggybackMessage* msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
        LOG("From casting:");
        _util.print(*msg2[0]);
        _util.print(*msg2[1]);

//        _util.copy(_msg, *msg2);
        COPY_PIGGYBACK_MESSAGE(_msg, *_msg2);

        LOG("Forwarder copying:");
        _util.print(_msg[0]);
        _util.print(_msg[1]);

        _util.decode(_msg, p);


        // Afterwards, kill the packet
        p->kill();
    }//else

    DEBUG("End Forwarder");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Forwarder)
