#include "FTAppenderElement.hh"
#include <clicknet/udp.h>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <click/packet_anno.hh>

CLICK_DECLS

#define ETHER_PACKET_MAX_SIZE 1500

FTAppenderElement::FTAppenderElement() {};

FTAppenderElement::~FTAppenderElement() {};

void FTAppenderElement::push(int source, Packet *p) {
    click_chatter("--------------------");
    click_chatter("In FTAppender element");
    click_chatter("Receiving packet %llu from port %d", getPacketId(p), source);

    if (source == FROM_DUMP) {
//        click_chatter("state on the packet going to state-element ");
//        printState(_temp);
        WritablePacket *q = encodeStates(p, _temp);
        _temp.clear();
        p->kill();
        output(TO_FT_STATE).push(q);
    }//if
    else if (source == FROM_TO_DEVICE) {
        try {
            FTPacketMBPiggyBackedState piggyBackedState;
            click_chatter("In appender-2");
//            decodeStates(p, piggyBackedState);
            decodeStatesRetPacket(p, piggyBackedState);
            click_chatter("the size of piggybacked state: %d", piggyBackedState.size());
            click_chatter("In appender-3");
            _temp.insert(piggyBackedState.begin(), piggyBackedState.end());
            printState(_temp);
        }//try
        catch(...) {
            click_chatter("In appender: Not valid packet for our protocol!");
        }//catch
        p->kill();
    }//else if

//    click_chatter("--------------------");
}

void FTAppenderElement::serializePiggyBacked(FTPacketMBPiggyBackedState &pbStates, stringstream &ss) {
    boost::archive::binary_oarchive oa(ss);
    oa << pbStates;
}

void FTAppenderElement::deserializePiggyBacked(stringstream &ss, FTPacketMBPiggyBackedState &piggyBackedStates) {
    boost::archive::binary_iarchive ia(ss);
    ia >> piggyBackedStates;
}

void FTAppenderElement::deserializePiggyBacked(string& states, FTPacketMBPiggyBackedState &piggyBackedStates) {
    stringstream ss(states);
    click_chatter("Deserialize stringstream size is: %d", ss.str().size());
    boost::archive::binary_iarchive ia(ss);
    click_chatter("Deserialize 1");
    ia >> piggyBackedStates;
    click_chatter("Deserialize 2");
}

void FTAppenderElement::serialize(FTMBStates &state, stringstream &ss) {
    boost::archive::binary_oarchive oa(ss);
    oa << state;
}

void FTAppenderElement::deserialize(stringstream &ss, FTMBStates &state) {
    boost::archive::binary_iarchive ia(ss);
    ia >> state;
}

int  FTAppenderElement::payloadOffset(Packet *p) {
    int off = -1;

    const click_ip *ip = p->ip_header();
    if (ip->ip_p == IP_PROTO_TCP) {
        off = p->transport_header_offset() + (p->tcp_header()->th_off << 2);
    }//if
    else if (ip->ip_p == IP_PROTO_UDP) {
        off = p->transport_header_offset() + sizeof(click_udp);
    }//else if
    else {
        off = p->transport_header_offset();
    }//else

    return off;
}

WritablePacket *FTAppenderElement::encodeStates(Packet *p, FTPacketMBPiggyBackedState &piggyBackedState) {
//    click_chatter("In encode state");
    stringstream stateSS;
    WritablePacket *q;

    try {
        serializePiggyBacked(piggyBackedState, stateSS);

        string compressed;
        compress(stateSS.str(), compressed);
        short stateLen = compressed.size();

        uint16_t newPacketSize = sizeof(short) + stateLen + p->length();
        q = Packet::make(newPacketSize);
        auto ploff = payloadOffset(p);

        memcpy(q->data(), p->data(), ploff);
        memcpy(q->data() + ploff, &stateLen, sizeof(short));
        memcpy(q->data() + ploff + sizeof(short), compressed.c_str(), stateLen);
        if (ploff < p->length())
            memcpy(q->data() + ploff + sizeof(short) + stateLen, p->data() + ploff, p->length() - ploff);


        click_ip *ip_header = reinterpret_cast<click_ip *>(q->data() + p->ip_header_offset());
        ip_header->ip_len = htons(ntohs(p->ip_header()->ip_len) + sizeof(short) + stateLen);
        ip_header->ip_sum = 0;

        int hlen = (ip_header->ip_hl) << 2;
        ip_header->ip_sum = click_in_cksum((unsigned char *) ip_header, hlen);

    }//try
    catch(...) {
        click_chatter("In encode state, in the catch block");
    }//catch

    return q;
}

int FTAppenderElement::decodeStates(Packet *p, FTPacketMBPiggyBackedState &piggyBackedState) {
    auto ploff = payloadOffset(p);

    short stateLen;
    memcpy(&stateLen, p->data() + ploff, sizeof(short));
    string states(reinterpret_cast<const char*>(p->data()) + ploff + sizeof(short), stateLen);
    string decompressed;
    decompress(states, decompressed);

    FTAppenderElement::deserializePiggyBacked(decompressed, piggyBackedState);

    return stateLen + sizeof(short);
}

WritablePacket* FTAppenderElement::decodeStatesRetPacket(Packet *p, FTPacketMBPiggyBackedState &piggyBackedState) {
    auto ploff = payloadOffset(p);

    click_chatter("Payload offset is: %d", ploff);

    short stateLen;
    memcpy(&stateLen, p->data() + ploff, sizeof(short));

    click_chatter("State length is: %d", stateLen);

    string states(reinterpret_cast<const char*>(p->data()) + ploff + sizeof(short), reinterpret_cast<const char*>(p->data()) + ploff + sizeof(short) + stateLen);
    string decompressed;
    decompress(states, decompressed);

    click_chatter("Decompressed size is: %d", decompressed.size());

    FTAppenderElement::deserializePiggyBacked(decompressed, piggyBackedState);

    int orgPayloadBegin = ploff + sizeof(short) + stateLen;
    click_chatter("p->length() - stateLen - sizeof(short) is: %d", p->length() - stateLen - sizeof(short));
    WritablePacket *q = Packet::make(p->length() - stateLen - sizeof(short));
    memcpy(q->data(), p->data(), ploff);
    memcpy(q->data() + ploff, p->data() + orgPayloadBegin, p->length() - orgPayloadBegin);

    click_ip* ip_header = reinterpret_cast<click_ip*>(q->data() + p->ip_header_offset());
    ip_header->ip_len = htons(ntohs(p->ip_header()->ip_len) - stateLen - sizeof(short));

    int hlen = (ip_header->ip_hl) << 2;
    ip_header->ip_sum = 0;
    ip_header->ip_sum = click_in_cksum((unsigned char *)ip_header, hlen);
    click_chatter("Hlen is: %d", hlen);

    return q;
}

void FTAppenderElement::compress(const std::string &data, std::string &buffer) {
    buffer.clear();
    boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
    out.push(boost::iostreams::zlib_compressor());
    out.push(boost::iostreams::back_inserter(buffer));
    boost::iostreams::copy(boost::make_iterator_range(data), out);
}

void FTAppenderElement::decompress(const std::string &data, std::string &buffer) {
    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(boost::make_iterator_range(data));
    buffer.clear();
    boost::iostreams::copy(in, boost::iostreams::back_inserter(buffer));
}

FTPacketId FTAppenderElement::getPacketId(Packet *p) {
    //TODO: implement an identifier for non tcp packets

    const click_ip* cip = reinterpret_cast<const click_ip*>(p->data() + 14);
    const click_tcp* tcp =  reinterpret_cast<const click_tcp*>(p->data() + 14 + 20);
    FTPacketId packetId = MAKE_UNIQUE_PACKET_ID(cip->ip_off, cip->ip_p, tcp->th_seq);

    return packetId;
}

void FTAppenderElement::printState(FTState &state) {
    for (auto it = state.begin(); it != state.end(); ++it) {
        click_chatter("%s: %s", it->first.c_str(), it->second.c_str());
    }//for
}

void FTAppenderElement::printState(FTPiggyBackedState &state) {
    click_chatter("Ack is %d, commit is %d", state.ack, state.commit);
    printState(state.state);
}

void FTAppenderElement::printState(FTPacketMBPiggyBackedState &state) {
    click_chatter("In print state:");
    for (auto i = state.begin(); i != state.end(); ++i) {
        click_chatter("State of packet %llu", i->first);
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            click_chatter("State of middlebox %d", (FTMBId)(j->first));
            printState(j->second);
        }//for
    }//for
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTAppenderElement)
ELEMENT_LIBS(-L/usr/local/lib -lboost_iostreams -lboost_serialization)