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

    _counter ++;

//    for (int i = 0; i < _counters.size(); ++i) {
//        stringstream value;
//        value << _counters[i];
//
//        stringstream key;
//        key << COUNTER << i;
//
//        stateElement->putPrimaryState(key.str(), value.str());
//    }//for

    if (_counter % _new_count == 0) {
        _counters.push_back(_counter);
    }

    LOG("End NFArrCounterMB %d:", _id);
    LOG("--------------------");

    return p;
}

int NFArrCounterMB::configure(Vector<String> &conf, ErrorHandler *errh) {
    IntArg parser;
    int size;

    parser.parse(conf[0], _id);

//    parser.parse(conf[1], size);
//
//    for (int i = 0; i < size; ++i) {
//        _counters.push_back(INIT_COUNTER);
//    }//for

    parser.parse(conf[1], _new_count);

    LOG("Counter MB id is %d!\n", _id);

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFArrCounterMB)