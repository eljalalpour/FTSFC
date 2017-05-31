#include "FTAppenderElement.hh"
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#include <click/args.hh>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <click/packet_anno.hh>

CLICK_DECLS

#define ETHER_PACKET_MAX_SIZE 1500

FTAppenderElement::FTAppenderElement() {};

FTAppenderElement::~FTAppenderElement() {};

int FTAppenderElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    BoundedIntArg parser(0, 0xFFF);
    parser.parse(conf[0], _first_vlan);
    LOG("First VLAN ID: %d", _first_vlan);

    return 0;
}

void FTAppenderElement::push(int source, Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTAppender element:");

    const click_ether_vlan *vlan = reinterpret_cast<const click_ether_vlan *>(p->data());
    VLANId vlan_id = (ntohs(vlan->ether_vlan_tci) & 0xFFF);
    DEBUG("VLAN-ID is %d", vlan_id);

    if (vlan_id == _first_vlan) {
        DEBUG("Receiving packet with size %d from the source!", p->length());
        DEBUG("state on the packet going to state-element");
        printState(_temp);
        WritablePacket *q = encodeStates(p, _temp);
        _temp.clear();

        p->kill();
        output(0).push(q);
    }//if
    else {
        DEBUG("Receiving packet from the end of the chain!");
        FTPiggyBackMessage msg;
        decodeStates(p, msg);

        printState(msg);

        append(msg);

        DEBUG("_temp size is %u", _temp.size());

        p->kill();
    }//else

    DEBUG("--------------------\n");
}

void FTAppenderElement::append(FTPiggyBackMessage state) {
    for(auto it = state.begin(); it != state.end(); ++it)
        _temp[it->first] = it->second;
}

void FTAppenderElement::serializePiggyBacked(FTPiggyBackMessage& msg, stringstream &ss) {
    boost::archive::binary_oarchive oa(ss);
    oa << msg;
}

void FTAppenderElement::deserializePiggyBacked(stringstream& ss, FTPiggyBackMessage& msg) {
    boost::archive::binary_iarchive ia(ss);
    ia >> msg;
}

void FTAppenderElement::deserializePiggyBacked(string& states, FTPiggyBackMessage& msg) {
    stringstream ss(states);
    boost::archive::binary_iarchive ia(ss);
    ia >> msg;
}

void FTAppenderElement::serialize(FTPiggyBackState &state, stringstream &ss) {
    boost::archive::binary_oarchive oa(ss);
    oa << state;
}

void FTAppenderElement::deserialize(stringstream &ss, FTPiggyBackState &state) {
    boost::archive::binary_iarchive ia(ss);
    ia >> state;
}

void FTAppenderElement::serialize(FTTimestampState &state, stringstream &ss) {
    boost::archive::binary_oarchive oa(ss);
    oa << state;
}

void FTAppenderElement::deserialize(stringstream &ss, FTTimestampState &state) {
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

WritablePacket *FTAppenderElement::encodeStates(Packet *p, FTPiggyBackMessage &msg) {
    stringstream stateSS;
    serializePiggyBacked(msg, stateSS);

    string compressed;
    compress(stateSS.str(), compressed);
    short stateLen = compressed.size();

    uint16_t newPacketSize = sizeof(short) + stateLen + p->length();
    WritablePacket *q = Packet::make(newPacketSize);
    int ploff = payloadOffset(p);

    memcpy(q->data(), p->data(), ploff);
    memcpy(q->data() + ploff, &stateLen, sizeof(short));
    memcpy(q->data() + ploff + sizeof(short), compressed.c_str(), stateLen);
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
    string decompressed;
    decompress(states, decompressed);

    FTAppenderElement::deserializePiggyBacked(decompressed, msg);

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

FTPacketId FTAppenderElement::getPacketId(Packet *p, int ip_offset) {
    //TODO: implement an identifier for non tcp packets

    const click_ip* cip  = reinterpret_cast<const click_ip*>(p->data() + ip_offset);
    const click_tcp* tcp = reinterpret_cast<const click_tcp*>(p->data() + ip_offset + 20);
    FTPacketId packetId  = MAKE_UNIQUE_PACKET_ID(cip->ip_off, cip->ip_p, tcp->th_seq);

    return packetId;
}

void FTAppenderElement::printState(FTState &state) {
    LOG("State size is %u", state.size());
    for (auto it = state.begin(); it != state.end(); ++it) {
        DEBUG("%s: %s", it->first.c_str(), it->second.c_str());
    }//for
}

void FTAppenderElement::printState(FTTimestampState &ft_state) {
    DEBUG("Timestamp %llu:", ft_state.timestamp);
    printState(ft_state.state);
}

void FTAppenderElement::printState(FTPiggyBackState &state) {
    DEBUG("Ack is %d, last commit is %llu",
          state.ack, state.last_commit);
    printState(state.state);
}

void FTAppenderElement::printState(FTPiggyBackMessage &msg) {
    for (auto i = msg.begin(); i != msg.end(); ++i) {
        DEBUG("\nState of middlebox %u", i->first);
        printState(i->second);
    }//for
}

void FTAppenderElement::encode(FTTimestampState& state, string& buffer) {
    stringstream ss;

    // Serialize and compress the state
    FTAppenderElement::serialize(state, ss);
    FTAppenderElement::compress(ss.str(), buffer);
}

void FTAppenderElement::decode(const string& buffer, FTTimestampState& state) {
    string decompressed;
    FTAppenderElement::decompress(buffer, decompressed);
    LOG("Decompressed size is: %d", decompressed.size());
    LOG("After decompress");

    stringstream ss;
    ss.str(decompressed);

    LOG("Before deserialize");
    FTAppenderElement::deserialize(ss, state);
}

void FTAppenderElement::decode(const char* data, int size, FTTimestampState& state) {
    string buffer(data, size);
    decode(buffer, state);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTAppenderElement)
ELEMENT_LIBS(-L/usr/local/lib -lboost_iostreams -lboost_serialization)