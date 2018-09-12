#include <click/config.h>
#include <click/router.hh>
#include "forwarder.hh"

CLICK_DECLS

Forwarder::Forwarder () {
    // Initialize message
    std::lock_guard<std::mutex> ll(_mutex);
    _util.init(_msg);
};

Forwarder::~Forwarder() { };

void Forwarder::push(int source, Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Forwarder");

    if (source == 0) { // Receiving a packet from the traffic source
        // Encode its memory of the piggyback message into the packet
        //TODO: make sure no lock is required for encoding and decoding
        {
            std::lock_guard<std::mutex> ll(_mutex);
            _util.print(_msg);
            _util.encode(_msg, p);
        }
        output(0).push(p);
    }//if
    else { //Receiving a packet from Buffer
        // Decode and memorize the piggyback message from the packet
        //TODO: make sure no lock is required for encoding and decoding
        {
            std::lock_guard<std::mutex> ll(_mutex);
            _util.decode(_msg, p);
        }

        // Afterwards, kill the packet
        p->kill();
    }//else

    DEBUG("End Forwarder");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Forwarder)
