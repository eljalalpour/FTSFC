#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include <click/packet_anno.hh>
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
        it = _packets.erase(it);
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

//    output(TO_FORWARDER).push(p->clone());
    output(TO_FORWARDER).push(Packet::make(p->data(), DEFAULT_OFFSET + sizeof(PiggybackMessage)));

    // Cast the piggyback message and extract the timestamps
    PiggybackMessage* _msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
    int64_t lts  = (*_msg[_chain_len - 1]).timestamp;
    int64_t lcts = (*_msg[_chain_len - 1]).last_commit;

    // Store the packet into buffer
    _packets[lts] = p;

    // Release packets from the buffer
    _release(lcts);

    DEBUG("End Buffer");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Buffer)
