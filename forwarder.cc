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

        auto msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
        for (int i = 0; i < _chain_len; ++i) {
            (*msg2[i]) = _msg[i];
        }//for

        output(0).push(p);
    }//if
    else { //Receiving a packet from Buffer
        // Decode and memorize the piggyback message from the packet

        auto msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
        for (int i = 0; i < _chain_len; ++i) {
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
