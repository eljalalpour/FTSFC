#include <click/config.h>
#include "FTBufferElement.hh"
#include <click/router.hh>
#include "FTAppenderElement.hh"

CLICK_DECLS

FTBufferElement::FTBufferElement() { }

FTBufferElement::~FTBufferElement() { }

void FTBufferElement::push(int, Packet *p) {
    click_chatter("------------------------------");
    click_chatter("Begin FTBufferElement");

    // Finding the packet id

    Router * router;

    // Writing the output of the last stage of the chain into the buffer
    FTPacketMBPiggyBackedState states;
    WritablePacket *q = FTAppenderElement::decodeStatesRetPacket(p, states);
    FTPacketId packetId = FTAppenderElement::getPacketId(q);
    click_chatter("q: Packetii %llu size %d", packetId, q->length());

    FTAppenderElement::printState(states);
    _packets[packetId] = q;

    // Send packet to the beginning of the chain
    click_chatter("--Before push--");
    output(TO_CHAIN_BEGIN).push(p);
    click_chatter("--After push---");

    click_chatter("Size of output buffer: %d", _packets.size());
    // Release the packets whose states have been committed
    for (auto it = states.begin(); it != states.end(); ++it) {
        auto oldPacketId = it->first;
        if (it->second.begin()->second.commit) {
            Packet *qq = _packets[oldPacketId];
            if (_packets.find(oldPacketId) == _packets.end()) {
                click_chatter("Packet %llu is not found!", oldPacketId);
            }//if
//            click_chatter("Packetyy pointer is %llu!", qq);
            click_chatter("packetyy %llu is released", oldPacketId);
            click_chatter("packet size %d", qq->length());
            output(TO_OUTSIDE_WORLD).push(qq);
            click_chatter("After push!");
            _packets.erase(oldPacketId);
            qq->kill();
            click_chatter("after erase!");
        }//if
    }//for

    click_chatter("End FTBufferElement");
    click_chatter("------------------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTBufferElement)