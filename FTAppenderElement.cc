#include "FTAppenderElement.hh"
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#include <click/args.hh>
#include <click/packet_anno.hh>

CLICK_DECLS

#define ETHER_PACKET_MAX_SIZE 1500
#define TIME_STAMP_OFFSET 16
#define HAS_TIME_STAMP true

FTAppenderElement::FTAppenderElement() {};

FTAppenderElement::~FTAppenderElement() {};

void FTAppenderElement::push(int source, Packet *p) {
//    DEBUG("--------------------");
//    DEBUG("Begin FTAppender element:");

    if (source == 0) {
        DEBUG("Receiving packet with size %d from the source!", p->length());
        DEBUG("state on the packet going to state-element");

//        printState(_temp);

        WritablePacket *q = encodeStates(p, _temp);
        _temp.clear();

        p->kill();
        output(0).push(q);
    }//if
    else {
//        DEBUG("Receiving packet from the end of the chain!");
        FTPiggyBackMessage msg;
        decodeStates(p, msg);

//        printState(msg);

        append(msg);

//        DEBUG("_temp size is %u", _temp.size());

        p->kill();
    }//else


    DEBUG("--------------------\n");
}

void FTAppenderElement::append(FTPiggyBackMessage& state) {
    for(auto it = state.begin(); it != state.end(); ++it)
        _temp[it->first] = it->second;
}

void FTAppenderElement::serializePiggyBacked(FTPiggyBackMessage& msg, string &ss) {
    Serializer _serializer;
    ss = _serializer.serialize(msg);
}

void FTAppenderElement::deserializePiggyBacked(string& ss, FTPiggyBackMessage& msg) {
    Serializer _serializer;
    _serializer.deserialize(msg, ss);
}

void FTAppenderElement::serialize(FTPiggyBackState &state, string &ss) {
    Serializer _serializer;
    ss = _serializer.serialize(state);
}

void FTAppenderElement::deserialize(string &ss, FTPiggyBackState &state) {
    Serializer _serializer;
    _serializer.deserialize(state, ss);
}

void FTAppenderElement::serialize(FTTimestampState &state, string &ss) {
    Serializer _serializer;
    ss = _serializer.serialize(state);
}

void FTAppenderElement::deserialize(string &ss, FTTimestampState &state) {
    Serializer _serializer;
    _serializer.deserialize(state, ss);
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

    if (HAS_TIME_STAMP && off + TIME_STAMP_OFFSET < p->length())
        off += TIME_STAMP_OFFSET;

    return off;
}

WritablePacket *FTAppenderElement::encodeStates(Packet *p, FTPiggyBackMessage &msg) {
    string stateSS;
    serializePiggyBacked(msg, stateSS);

    short stateLen = stateSS.size();

    uint16_t newPacketSize = sizeof(short) + stateLen + p->length();
    WritablePacket *q = Packet::make(newPacketSize);
    int ploff = payloadOffset(p);

    memcpy(q->data(), p->data(), ploff);
    memcpy(q->data() + ploff, &stateLen, sizeof(short));
//    memcpy(q->data() + ploff + sizeof(short), compressed.c_str(), stateLen);
    memcpy(q->data() + ploff + sizeof(short), stateSS.c_str(), stateLen);
    if (ploff < p->length())
        memcpy(q->data() + ploff + sizeof(short) + stateLen, p->data() + ploff, p->length() - ploff);

    click_ip* ip_header = reinterpret_cast<click_ip*>(q->data() + p->ip_header_offset());
    ip_header->ip_len = htons(ntohs(p->ip_header()->ip_len) + sizeof(short) + stateLen);
    ip_header->ip_sum = 0;

    int hlen = (ip_header->ip_hl) << 2;
    ip_header->ip_sum = click_in_cksum((unsigned char *)ip_header, hlen);

    return q;
}

int FTAppenderElement::decodeStates(Packet *p, FTPiggyBackMessage &msg) {
    auto ploff = payloadOffset(p);

    short stateLen;
    memcpy(&stateLen, p->data() + ploff, sizeof(short));
    string states(reinterpret_cast<const char*>(p->data()) + ploff + sizeof(short), stateLen);
    FTAppenderElement::deserializePiggyBacked(states, msg);

    return stateLen + sizeof(short);
}

WritablePacket* FTAppenderElement::decodeStatesRetPacket(Packet *p, FTPiggyBackMessage &msg) {
    auto ploff = payloadOffset(p);
    int stateLenPShort = FTAppenderElement::decodeStates(p, msg);
    int orgPayloadBegin = ploff + stateLenPShort;
    WritablePacket *q = Packet::make(p->length() - stateLenPShort);
    memcpy(q->data(), p->data(), ploff);
    memcpy(q->data() + ploff, p->data() + orgPayloadBegin, p->length() - orgPayloadBegin);

    click_ip* ip_header = reinterpret_cast<click_ip*>(q->data() + p->ip_header_offset());
    ip_header->ip_len = htons(ntohs(p->ip_header()->ip_len) - stateLenPShort);

    int hlen = (ip_header->ip_hl) << 2;
    ip_header->ip_sum = 0;
    ip_header->ip_sum = click_in_cksum((unsigned char *)ip_header, hlen);

    return q;
}

FTPacketId FTAppenderElement::getPacketId(Packet *p, int ip_offset) {
    //TODO: implement an identifier for non tcp packets

    const click_ip* cip  = reinterpret_cast<const click_ip*>(p->data() + ip_offset);
    const click_tcp* tcp = reinterpret_cast<const click_tcp*>(p->data() + ip_offset + 20);
    FTPacketId packetId  = MAKE_UNIQUE_PACKET_ID(cip->ip_off, cip->ip_p, tcp->th_seq);

    return packetId;
}

void FTAppenderElement::printState(FTState &state) {
    #ifndef DEBUG
    #ifndef LOG
        return;
    #endif
    #endif

    LOG("State size is %u", state.size());
    for (auto it = state.begin(); it != state.end(); ++it) {
        DEBUG("%s: %s", it->first.c_str(), it->second.c_str());
    }//for
}

void FTAppenderElement::printState(FTTimestampState &ft_state) {
#ifndef DEBUG
    #ifndef LOG
        return;
    #endif
#endif

    DEBUG("Timestamp %llu:", ft_state.timestamp);
    printState(ft_state.state);
}

void FTAppenderElement::printState(FTPiggyBackState &state) {
#ifndef DEBUG
    #ifndef LOG
        return;
    #endif
#endif
    DEBUG("Ack is %d, last commit is %llu",
          state.ack, state.last_commit);
    printState(state.state);
}

void FTAppenderElement::printState(FTPiggyBackMessage &msg) {
#ifndef DEBUG
    #ifndef LOG
        return;
    #endif
#endif

    for (auto i = msg.begin(); i != msg.end(); ++i) {
        DEBUG("\nState of middlebox %u", i->first);
        printState(i->second);
    }//for
}

void FTAppenderElement::encode(FTTimestampState& state, string& buffer) {
    Serializer _serializer;
    buffer = _serializer.serialize(state);
}

void FTAppenderElement::decode(const string& buffer, FTTimestampState& state) {
    Serializer _serializer;
    _serializer.deserialize(state, buffer);
}

void FTAppenderElement::decode(const char* data, int size, FTTimestampState& state) {
    string buffer(data, size);
    decode(buffer, state);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTAppenderElement)
ELEMENT_LIBS(-L/usr/local/lib -lprotobuf)