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
    //LOG("front: %llu, commit: %llu", _timestamps.front(), commit_timestamp);
    
    while(!_timestamps.empty()) {
        if (_timestamps.front() > commit_timestamp)
            break;

        output(TO_OUTSIDE_WORLD).push(_packets.front());

        _timestamps.pop();
        _packets.pop();
    }//for
}

void Buffer::_send_to_forwarder(Packet*& p) {
    if (_batch_counter++ % _batch_size != 0)
        return;
    
    // Zero copy packet creation
    auto q = Packet::make(CAST_AWAY_PACKET_DATA(p), TO_FORWARDER_PKT_SIZE, Util::no_op_pkt_destructor);
    click_ip *ip = reinterpret_cast<click_ip *>(q->data() + MAC_HEAD_SIZE);
    click_udp *udp = reinterpret_cast<click_udp *>(ip + UDP_HEAD_OFFSET_AFTER_MAC_HEAD);

    // store the ip header
    memcpy(&_iph, ip, sizeof(click_ip));

    udp->uh_ulen = htons(q->length() - MAC_HEAD_SIZE - sizeof(click_ip));
    udp->uh_sum = DEFAULT_CRC;
    ip->ip_hl = sizeof(click_ip) >> 2;
    ip->ip_len = htons(q->length() - MAC_HEAD_SIZE);
    ip->ip_sum = DEFAULT_CRC;

    output(TO_FORWARDER).push(q);

    // restore the ip header
    memcpy(ip, &_iph, sizeof(click_ip));

//    auto msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
//    auto msg2 = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(q);
//    LOG("p's 0 timestamp: %llu", msg[0]->timestamp);
//    LOG("q's 0 timestamp: %llu", msg2[0]->timestamp);
//    LOG("p's 0 last commit: %llu", msg[0]->last_commit);
//    LOG("q's 0 last commit: %llu", msg2[0]->last_commit);
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
    _release((*_msg[0]).last_commit);

    // Store the packet into buffer
    if (_packets.size() < MAX_BUFFER_SIZE) {
        _timestamps.push((*_msg[0]).timestamp);
        _packets.push(p);
    }//if
    else {
        // if there is not enough space for buffer
        //drop the packet
        p->kill();
    }//else

//    output(0).push(p);

    DEBUG("End Buffer");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Buffer)
