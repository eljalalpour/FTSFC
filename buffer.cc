#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "buffer.hh"

CLICK_DECLS

Buffer::Buffer () { };

Buffer::~Buffer() { };

void Buffer::_release(int64_t commit_timestamp) {
//    int count = 0;
    for (auto it = _packets.begin(); it != _packets.end(); /* no increment */) {
        if (it->first > commit_timestamp)
            break;

        output(TO_OUTSIDE_WORLD).push(it->second);
        _packets.erase(it++);

//        ++count;
    }//for

//    LOG("Packets in buffer: %d, Released Packets: %d!", _packets.size(), count);
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
    _msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
    int64_t lts  = (*_msg[_chain_len - 1]).timestamp;
    int64_t lcts = (*_msg[_chain_len - 1]).last_commit;

//    Util util;
//    LOG("The state of MB %d:", _chain_len - 1);
//    util.print((*_msg[_chain_len - 1]));

    // Store the packet into buffer
//    _packets[lts] = Packet::make(p->data(), p->length());
    _packets[lts] = p->uniqueify();

    //TODO: find the best order, either first send the packet to Forwarder or release packets
    // Send a copy of the packet to Forwarder
    DEBUG("Send to forwarder");
    output(TO_FORWARDER).push(p);

    // Release packets in the buffer
    DEBUG("Releasing!");
    _release(lcts);

    DEBUG("End Buffer");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Buffer)
