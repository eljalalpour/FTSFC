#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include <click/packet_anno.hh>
#include "buffer.hh"

CLICK_DECLS

Buffer::Buffer () : _q(0) {
    _timestamps.reserve(INIT_TIMESTAMPS_SIZE);
};

Buffer::~Buffer() { };

int Buffer::initialize(ErrorHandler *errh)  {
    assert(!_q && head() == 0 && tail() == 0);
    _q = (Packet **) CLICK_LALLOC(sizeof(Packet *) * (INIT_TIMESTAMPS_SIZE + 1));
    if (_q == 0)
        return errh->error("out of memory");
    return 0;
}


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

    output(TO_FORWARDER).push(p->clone());

    // Cast the piggyback message and extract the timestamps
    PiggybackMessage* _msg = CAST_PACKET_TO_PIGGY_BACK_MESSAGE(p);
    int64_t lts  = (*_msg[_chain_len - 1]).timestamp;
    int64_t lcts = (*_msg[_chain_len - 1]).last_commit;

//    // Store the packet into buffer
//    _packets[lts] = p;
//
//    // Release packets from the buffer
//    _release(lcts);

    _timestamps.push_back(lts);

    // If you change this code, also change NotifierQueue::push()
    // and FullNoteQueue::push().
    Storage::index_type h = head(), t = tail(), nt = next_i(t);

    // should this stuff be in SimpleQueue::enq?
    if (nt != h) {
        _q[t] = p;
        set_tail(nt);

        int s = size(h, nt);
        if (s > _highwater_length)
            _highwater_length = s;

    } else {
        // if (!(_drops % 100))
//        checked_output_push(1, p);
    }

    DEBUG("End Buffer");
    DEBUG("--------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Buffer)
