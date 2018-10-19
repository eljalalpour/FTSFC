#pragma once

#include "defs.hh"
#include <map>
#include <queue>
#include <click/config.h>
#include <click/element.hh>

CLICK_DECLS

///
/// Because of the same argument as of \texttt{Forwarder}'s,
/// \texttt{Buffer} threads can execute independently.
///

#define TO_OUTSIDE_WORLD 0
#define TO_FORWARDER     1
#define PAD              16
#define MAC_HEAD_SIZE    14
#define UDP_HEAD_OFFSET_AFTER_MAC_HEAD 1
#define DEFAULT_CRC 0

class Buffer : public Element {
private:
    int _chain_len;
    int _batch_counter;
    int _batch_size;

    std::queue<int64_t> _timestamps;
    std::queue<Packet*> _packets;

    inline void _release(int64_t);
    inline void _send_to_forwarder(Packet*);

public:
    Buffer ();

    ~Buffer();

    const char *class_name() const { return "Buffer"; }

    const char *port_count() const { return PORTS_1_2; }

    const char *processing() const { return AGNOSTIC; }

    void push(int, Packet*);

    int configure(Vector<String> &, ErrorHandler *);

    size_t length();
};

CLICK_ENDDECLS