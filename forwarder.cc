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

    if (p->length() < DEFAULT_OFFSET + sizeof(PiggybackMessage)) {
        LOG("Smaller!");
    }//if

    if (source == 0) { // Receiving a packet from the traffic source
        // Encode its memory of the piggyback message into the packet
        //TODO: make sure no lock is required for encoding and decoding
        auto msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
        for (int i = 0; i < MB_LEN; ++i) {
            (*msg2[i]) = _msg[i];
        }//for

        output(0).push(p);
    }//if
    else { //Receiving a packet from Buffer
        // Decode and memorize the piggyback message from the packet
        //TODO: make sure no lock is required for encoding and decoding

        auto msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
        for (int i = 0; i < MB_LEN; ++i) {
            _msg[i] = (*msg2[i]);
        }//for

        // Afterwards, kill the packet
        p->kill();
    }//else

    DEBUG("End Forwarder");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Forwarder)
