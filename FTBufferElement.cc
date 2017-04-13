#include <click/config.h>
#include "FTBufferElement.hh"
#include <click/router.hh>
#include "FTAppenderElement.hh"

CLICK_DECLS

FTBufferElement::FTBufferElement() { }

FTBufferElement::~FTBufferElement() { }

void FTBufferElement::push(int, Packet *p) {
    click_chatter("------------------------------");
    click_chatter("This is Buffer");

    // Finding the packet id
    auto packetId = FTAppenderElement::getPacketId(p);


    // Writing the output of the last stage of the chain into the buffer
    FTPacketMBPiggyBackedState states;
    WritablePacket *q = FTAppenderElement::decodeStatesRetPacket(p,states);
    FTAppenderElement::printState(states);
    _packets[packetId] = q;


    // Send it to the beginning of the chain
    output(TO_CHAIN_BEGIN).push(p);

    click_chatter("this is the size of output buffer %d", _packets.size());
    // Release the packets whose states have been committed
    for (auto it = states.begin(); it != states.end(); ++it) {
            auto oldPacketId = it->first;
            if (it->second.begin()->second.commit) {
                    Packet *qq = _packets[oldPacketId];
                    click_chatter("packet %d is released", oldPacketId);
                    output(TO_OUTSIDE_WORLD).push(qq);
                    _packets.erase(oldPacketId);
                }//if
        }//for
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTBufferElement)