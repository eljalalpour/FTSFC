#include "FTStateElement.hh"
#include "FTAppenderElement.hh"
#include <click/config.h>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "NFArrCounterMB.hh"

CLICK_DECLS

NFArrCounterMB::NFArrCounterMB() {};

NFArrCounterMB::~NFArrCounterMB() {};

Packet *NFArrCounterMB::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin NFArrCounterMB %d:", _id);

    _state = _state;

    LOG("End NFArrCounterMB %d:", _id);
    LOG("--------------------");

    return p;
}

int NFArrCounterMB::configure(Vector<String> &conf, ErrorHandler *errh) {
    IntArg parser;
    int size;

    parser.parse(conf[0], _id);
    parser.parse(conf[1], size);
    _state = string(size, 'n');

    LOG("NF Counter MB id is %d!\n", _id);

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFArrCounterMB)