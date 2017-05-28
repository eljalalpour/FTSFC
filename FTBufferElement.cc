#include <click/config.h>
#include "FTBufferElement.hh"
#include "FTAppenderElement.hh"

CLICK_DECLS

FTBufferElement::FTBufferElement() : _all(0), _released(0) { }

FTBufferElement::~FTBufferElement() { }

void FTBufferElement::push(int, Packet *p) {
    DEBUG("\n------------------------------");
    DEBUG("Begin FTBufferElement\n\n");

    // TODO: remove-start
    ++_all;
    // TODO: remove-end

    // Writing the output of the last stage of the chain into the buffer
    FTPacketMBPiggyBackedState states;

    FTPacketId pid1 = FTAppenderElement::getPacketId(p);
    WritablePacket *q = FTAppenderElement::decodeStatesRetPacket(p, states);
    FTPacketId pid2 = FTAppenderElement::getPacketId(p);

    FTPacketId packetId = FTAppenderElement::getPacketId(q);

    click_chatter("1: %llu, 2: %llu, 3: %llu", pid1, pid2, packetId);

    DEBUG("Packet %llu with the size of %d (packet + state is %d) pushed to buffer",
          packetId, q->length(), p->length());

    FTAppenderElement::printState(states);
    _packets[packetId] = q;
//    _packets[packetId] = p;

    // Send packet to the beginning of the chain
    DEBUG("Packet %llu with size %d sent to the beginning of the chain (on port %d)!",
          packetId, p->length(), TO_CHAIN_BEGIN);
    output(TO_CHAIN_BEGIN).push(p);
    DEBUG("After Pushing packet %llu", packetId);

//    vector<FTPacketId> released_packets;

    DEBUG("Size of output buffer: %d", _packets.size());

    // Release the packets whose states have been committed
    for (auto it = states.begin(); it != states.end(); ++it) {
        auto oldPacketId = it->first;

        // TODO: remove-start
        auto ctr = _checked_to_release.find(oldPacketId);
        if (ctr == _checked_to_release.end())
            _checked_to_release[oldPacketId] = 1;
        else
            ctr->second++;
        DEBUG("%d-th time, packet %llu is being checked for release!",
              _checked_to_release[oldPacketId], oldPacketId);
        // TODO: remove-end

        if (it->second.begin()->second.commit) {

            auto item = _packets.find(oldPacketId);
            if (item == _packets.end()) {
                DEBUG("Packet %llu is not found!", oldPacketId);
                continue;
            }//if

            Packet *qq = item->second;
            click_chatter("packet %llu with the size of %d is going to be released", oldPacketId, qq->length());
            output(TO_OUTSIDE_WORLD).push(qq);

            qq->kill();
            _packets.erase(oldPacketId);

//            released_packets.push_back(oldPacketId);
//            _count += released_packets.size();
            // TODO: remove-start
            ++_released;
            // TODO: remove-end

//            DEBUG("after erase!");
        }//if
        // TODO: remove-start
        else {
            DEBUG("Packet %llu not committed yet to be released!", oldPacketId);
        }//else
        // TODO: remove-end
    }//for

    // TODO: remove-start
    int rct_count = 0;
    for (auto it = _checked_to_release.begin(); it != _checked_to_release.end(); ++it)
        rct_count += it->second;
    DEBUG("Received packets: %d, identical checked for release: %d, all checked for release: %d, released packets: %d",
                  _all, _checked_to_release.size(), rct_count, _released);
    // TODO: remove-end

    DEBUG("\nEnd FTBufferElement");
    DEBUG("------------------------------\n");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTBufferElement)