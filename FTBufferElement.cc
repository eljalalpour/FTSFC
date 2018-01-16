#include <click/config.h>
#include "FTBufferElement.hh"
#include "FTAppenderElement.hh"

CLICK_DECLS

FTBufferElement::FTBufferElement() { }

FTBufferElement::~FTBufferElement() { }

FTTimestamp FTBufferElement::last_timestamp(FTPiggyBackMessage& msg) {
    return msg.rbegin()->second.timestamp;
}

FTTimestamp FTBufferElement::last_commit_timestamp(FTPiggyBackMessage& msg) {
    return msg.rbegin()->second.last_commit;
}

void FTBufferElement::release(FTTimestamp commit_timestamp) {
    DEBUG("Releasing packets, packets length is %d!", _packets.size());
    int i = 0;
    for (auto it = _packets.begin(); it != _packets.end(); /* no increment */) {
	    DEBUG("Check packet!");
        if (it->first > commit_timestamp)
            break;
	
//        DEBUG("Release packet %d-th", i);
//        FTPiggyBackMessage msg;
//        FTAppenderElement::decodeStates(it->second, msg);
//        FTAppenderElement::printState(msg);

        output(TO_OUTSIDE_WORLD).push(it->second);
    	DEBUG("Erase the packet %d-th", i);
        _packets.erase(it++);
	    i++;
    }//for
    DEBUG("End of releasing packets!");
}

void FTBufferElement::push(int, Packet *p) {
////    DEBUG("\n------------------------------");
////    DEBUG("Begin FTBufferElement\n\n");
//
//    // Writing the output of the last stage of the chain into the buffer
//    FTPiggyBackMessage msg;
//
//    WritablePacket *q = FTAppenderElement::decodeStatesRetPacket(p, msg);
//
////    DEBUG("Packet with the size of %d (packet + state is %d) pushed to buffer",
////          q->length(), p->length());
//
////    FTAppenderElement::printState(msg);
//
//    auto lts = last_timestamp(msg);
//    _packets[lts] = q;
//
//    // Send packet to the beginning of the chain
////    DEBUG("Packet with size %d sent to the beginning of the chain (on port %d)!",
////          p->length(), TO_CHAIN_BEGIN);
//    output(TO_CHAIN_BEGIN).push(p);
//
//    DEBUG("Size of output buffer: %d", _packets.size());
//
//    // Release the packets whose timestamp is less than the last commit timestamp
//    release(last_commit_timestamp(msg));
//
//    DEBUG("\nEnd FTBufferElement");
//    DEBUG("------------------------------\n");

    DEBUG("\n------------------------------");
    DEBUG("Begin FTBufferElement\n\n");

    LOG("Buffer size is: %d", _packets.size());
    
    FTPiggyBackMessage msg;
    FTAppenderElement::decodeStates(p, msg);
    
    auto lts = last_timestamp(msg);
    _packets[lts] = FTAppenderElement::stripStates(p);
//    _packets[lts] = Packet::make(p->data(), p->length());
    
    output(TO_CHAIN_BEGIN).push(p);
    release(last_commit_timestamp(msg));

    DEBUG("\nEnd FTBufferElement");
    DEBUG("------------------------------\n");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTBufferElement)
