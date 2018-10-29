#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include <click/packet_anno.hh>
#include <clicknet/ip.h>
#include <clicknet/udp.h>
#include "buffer.hh"

CLICK_DECLS

Buffer::Buffer () : _batch_counter(0) { };

Buffer::~Buffer() { };

void Buffer::_release(int64_t commit_timestamp) {
    while(!_timestamps.empty()) {
        if (_timestamps.front() > commit_timestamp)
            break;

        output(TO_OUTSIDE_WORLD).push(_packets.front());

        _timestamps.pop();
        _packets.pop();
    }//for
}

void Buffer::_send_to_forwarder(Packet* p) {
    if (_batch_counter++ % _batch_size != 0)
        return;

    auto n = DEFAULT_OFFSET + sizeof(PiggybackMessage) + PAD;
    auto q = Packet::make(p->data(), n);
    click_ip *ip = reinterpret_cast<click_ip *>(q->data() + MAC_HEAD_SIZE);
    click_udp *udp = reinterpret_cast<click_udp *>(ip + UDP_HEAD_OFFSET_AFTER_MAC_HEAD);
    udp->uh_ulen = htons(q->length() - MAC_HEAD_SIZE - sizeof(click_ip));
    udp->uh_sum = DEFAULT_CRC;
    ip->ip_hl = sizeof(click_ip) >> 2;
    ip->ip_len = htons(q->length() - MAC_HEAD_SIZE);
    ip->ip_sum = DEFAULT_CRC;

    output(TO_FORWARDER).push(q);
}

int Buffer::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("CHAIN", _chain_len)
                .read("BATCH", _batch_size)
                .complete() < 0)
        return -1;

    LOG("Chain length is %d and batch is %d!\n", _chain_len, _batch_size);

    return 0;
}

size_t Buffer::length() {
    return _packets.size();
}

void Buffer::push(int, Packet*p) {
    DEBUG("--------------------");
    DEBUG("Begin Buffer");

    _send_to_forwarder(p);

    // Cast the piggyback message and extract the timestamps
    PiggybackMessage* _msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);

    // Release packets from the buffer
    _release((*_msg[_chain_len - 1]).last_commit);

    // Store the packet into buffer
    _timestamps.push((*_msg[_chain_len - 1]).timestamp);
    _packets.push(p);

    DEBUG("End Buffer");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Buffer)
