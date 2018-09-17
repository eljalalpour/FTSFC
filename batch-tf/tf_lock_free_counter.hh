#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include "transmitter.hh"

CLICK_DECLS

#define DEFAULT_INDEX 0
#define DEFAULT_VALUE 1

class TFLockFreeCounter : public Element {
private:
    int _index;
    bool _trans_init;

    Transmitter *_trans;

    Packet* _queue[QUEUE_LEN];
    int _queued_packets;

    inline void _init_transmitter();

public:
    TFLockFreeCounter  ();

    ~TFLockFreeCounter ();

    const char *class_name() const { return "TFLockFreeCounter "; }

    const char *port_count() const { return PORTS_1_1; }

    const char *processing() const { return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);

    void push(int, Packet*);
};

CLICK_ENDDECLS