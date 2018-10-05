#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include <click/packet_anno.hh>
#include <clicknet/ip.h>
#include <click/etheraddress.hh>
#include "buffer.hh"

CLICK_DECLS

Buffer::Buffer () { };

Buffer::~Buffer() { };

void Buffer::_release(int64_t commit_timestamp) {
    LOG("Buffer length: %d", _timestamps.size());
    while(!_timestamps.empty()) {
        if (_timestamps.front() > commit_timestamp)
            break;

        output(TO_OUTSIDE_WORLD).push(_packets.front());

        _timestamps.pop();
        _packets.pop();
    }//for
}

int Buffer::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("CHAIN", _chain_len)
                .complete() < 0)
        return -1;

    DEBUG("Chain length is %d!\n", _chain_len);

    return 0;
}

size_t Buffer::length() {
    return _packets.size();
}

void Buffer::push(int, Packet*p) {
    DEBUG("--------------------");
    DEBUG("Begin Buffer");

    // Cast the piggyback message and extract the timestamps
    PiggybackMessage* _msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // Release packets from the buffer
    _release((*_msg[_chain_len - 1]).last_commit);

    // Store the packet into buffer
    _timestamps.push((*_msg[_chain_len - 1]).timestamp);
    _packets.push(p);

//    output(TO_FORWARDER).push(p->clone());
    int n = DEFAULT_OFFSET + sizeof(PiggybackMessage);
    auto q = Packet::make(p->data(), n);
    PiggybackMessage* _msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(q);
    for (int i = 0; i < _chain_len; ++i) {
        (*_msg2[i]) = (*_msg[i]);
    }// for

    click_ip *ip = reinterpret_cast<click_ip *>(q->data() + 14);
    click_udp *udp = reinterpret_cast<click_udp *>(ip + 1);
    udp->uh_ulen = htons(q->length() - 14 - sizeof(click_ip));
    udp->uh_sum = 0;
    ip->ip_hl = sizeof(click_ip) >> 2;
    ip->ip_len = htons(q->length() - 14);
    ip->ip_sum = 0;
//    ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));

    _msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(q);
    LOG("Message2 timestamp: %llu", (*_msg2[_chain_len - 1]).timestamp);
    output(TO_FORWARDER).push(q);

    DEBUG("End Buffer");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Buffer)