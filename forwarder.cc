#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "forwarder.hh"

CLICK_DECLS

Forwarder::Forwarder () {
    // Initialize message
    _util.init(_msg);
};

Forwarder::~Forwarder() { };

int Forwarder::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
                .read("CHAIN", _chain_len)
                .complete() < 0)
        return -1;

    LOG("Forwarder: Chain length is %d!\n", _chain_len);

    _copy_len = _chain_len * sizeof(PiggybackState);

    return 0;
}

void Forwarder::push(int source, Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Forwarder");

    if (source == 0) { // Receiving a packet from the traffic source
        // Encode its memory of the piggyback message into the packet
        //TODO: make sure no lock is required for encoding and decoding
        auto msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
        memcpy(msg2, _msg, _copy_len);

        output(0).push(p);
    }//if
    else { //Receiving a packet from Buffer
        // Decode and memorize the piggyback message from the packet
        //TODO: make sure no lock is required for encoding and decoding

        auto msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
        memcpy(_msg, msg2, _copy_len);
        // Afterwards, kill the packet
        p->kill();
    }//else

    DEBUG("End Forwarder");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Forwarder)
