#include "FTAppenderElement.hh"
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#include <click/args.hh>
#include <click/packet_anno.hh>

CLICK_DECLS

#define ETHER_PACKET_MAX_SIZE 1500
#define DEFAULT_OFFSET 76 // This value must be greater than 75
#define STATE_ROOM 200

FTAppenderElement::FTAppenderElement() {};

FTAppenderElement::~FTAppenderElement() {};

void FTAppenderElement::push(int source, Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTAppender element:");

    if (source == 0) {
        DEBUG("Receiving packet with size %d from the source!", p->length());
        DEBUG("state on the packet going to state-element");

        //printState(_temp);
        _lock.lock();
        WritablePacket *q = encodeStates(p, _temp);
        //LOG("after encode state in FTAppender, q: %llu", q);
        _temp.clear();
        _lock.unlock();
        
        //p->kill();
        if (q != 0) {
            output(0).push(q);
        }//if
        p->kill();
    }//if
    else {
        DEBUG("Receiving packet from the end of the chain!");
        FTPiggyBackMessage msg;
        decodeStates(p, msg);

        //printState(msg);
        _lock.lock();
        append(msg);
        _lock.unlock();
        //DEBUG("_temp size is %u", _temp.size());

        p->kill();
    }//else

    DEBUG("End FTAppender element:");
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

    return off;
}

WritablePacket *FTAppenderElement::encodeStates(Packet *p, FTPiggyBackMessage &msg) {
//    string stateSS;
//    serializePiggyBacked(msg, stateSS);

//    String _data(stateSS.c_str(), stateSS.size());
//    int stateLen = _data.length();
//    int new_pkt_size = p->length() + stateLen + sizeof(stateLen);
//    WritablePacket *q = Packet::make(new_pkt_size);

//    memcpy(q->data(), p->data(), DEFAULT_OFFSET);
//    memcpy(q->data() + DEFAULT_OFFSET, &stateLen, sizeof(stateLen));
//    memcpy(q->data() + DEFAULT_OFFSET + sizeof(stateLen), _data.data(), stateLen);
//    memcpy(q->data() + DEFAULT_OFFSET + sizeof(stateLen) + stateLen, p->data() + DEFAULT_OFFSET, p->length() - DEFAULT_OFFSET);

//    return q;

    string stateSS;
    serializePiggyBacked(msg, stateSS);

    String _data(stateSS.c_str(), stateSS.size());
    int stateLen = _data.length();

    if (stateLen > STATE_ROOM) {
        auto dd = "State length is bigger than STATE_ROOM!";
        DEBUG(dd);
        throw dd;
    }//if

    int new_pkt_size = p->length() + STATE_ROOM;
    WritablePacket *q = Packet::make(new_pkt_size);

    memcpy(q->data(), p->data(), DEFAULT_OFFSET);
    memcpy(q->data() + DEFAULT_OFFSET, &stateLen, sizeof(stateLen));
    memcpy(q->data() + DEFAULT_OFFSET + sizeof(stateLen), _data.data(), stateLen);
    memcpy(q->data() + DEFAULT_OFFSET + STATE_ROOM, p->data() + DEFAULT_OFFSET, p->length() - DEFAULT_OFFSET);

    return q;
}

void FTAppenderElement::lightEncodeStates(Packet *p, FTPiggyBackMessage &msg) {
    string stateSS;
    serializePiggyBacked(msg, stateSS);

    String _data(stateSS.c_str(), stateSS.size());
    int stateLen = _data.length();

    unsigned char * p_data = const_cast<unsigned char *>(p->data());

    memcpy(p_data + DEFAULT_OFFSET, &stateLen, sizeof(stateLen));
    memcpy(p_data + DEFAULT_OFFSET + sizeof(stateLen), _data.data(), stateLen);
}

int FTAppenderElement::decodeStates(Packet *p, FTPiggyBackMessage &msg) {
    int stateLen;
    memcpy(&stateLen, p->data() + DEFAULT_OFFSET, sizeof(stateLen));
    String _data(p->data() + DEFAULT_OFFSET + sizeof(stateLen), stateLen); 

    string statesStr(_data.data(), _data.length());
    FTAppenderElement::deserializePiggyBacked(statesStr, msg);

    return stateLen + sizeof(stateLen);
}

WritablePacket* FTAppenderElement::stripStates(Packet *p) {
    WritablePacket* q = Packet::make(p->length() - STATE_ROOM);

    memcpy(q->data(), p->data(), DEFAULT_OFFSET);
    memcpy(q->data() + DEFAULT_OFFSET,
           p->data() + DEFAULT_OFFSET + STATE_ROOM,
           p->length() - DEFAULT_OFFSET - STATE_ROOM);

    return q;
}

WritablePacket* FTAppenderElement::decodeStatesRetPacket(Packet *p, FTPiggyBackMessage &msg) {
    auto ploff = DEFAULT_OFFSET;
//    int stateLenPShort = FTAppenderElement::decodeStates(p, msg);
//    int orgPayloadBegin = ploff + stateLenPShort;
//    WritablePacket *q = Packet::make(p->length() - stateLenPShort);
//    memcpy(q->data(), p->data(), ploff);
//    memcpy(q->data() + ploff, p->data() + orgPayloadBegin, p->length() - orgPayloadBegin);
//
//    click_ip* ip_header = reinterpret_cast<click_ip*>(q->data() + p->ip_header_offset());
//    ip_header->ip_len = htons(ntohs(p->ip_header()->ip_len) - stateLenPShort);
//
//    int hlen = (ip_header->ip_hl) << 2;
//    ip_header->ip_sum = 0;
//    ip_header->ip_sum = click_in_cksum((unsigned char *)ip_header, hlen);
//
//    return q;

    FTAppenderElement::decodeStates(p, msg);
    return p->uniqueify();
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

WritablePacket* FTAppenderElement::strip(Packet *p, int offset, int size) {
    int new_pkt_size = p->length() - size;
    WritablePacket *q = Packet::make(new_pkt_size);

    memcpy(q->data(), p->data(), offset);
    memcpy(q->data() + offset, p->data() + offset + size, p->length() - offset - size);

    return q;
}

WritablePacket* FTAppenderElement::extend(Packet *p, int offset, int size) {
    int new_pkt_size = p->length() + size;
    WritablePacket *q = Packet::make(new_pkt_size);

    memcpy(q->data(), p->data(), offset);
    memcpy(q->data() + offset + size, p->data() + offset, p->length() - offset);

    return q;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTAppenderElement)
ELEMENT_LIBS(-L/usr/local/lib -lprotobuf -pthread -lpthread)
