#include "FTStateElement.hh"
#include "FTAppenderElement.hh"
#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "Transmitter.hh"
#include "TrArrCounterMB.hh"

CLICK_DECLS

TrArrCounterMB::TrArrCounterMB() {};

TrArrCounterMB::~TrArrCounterMB() {};

Packet *TrArrCounterMB::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin TrArrCounterMB %d:", _id);
    Router *r = this->router();

    //TODO: change the name of stateElement back to se
    Transmitter *trans = (Transmitter *)(r->find("trans"));
    FTState state;

    _counter ++;

    if (_counter % _new_count == 0) {
        _counters.push_back(_counter);
    }//if

    for (int i = 0; i < _counters.size(); ++i) {
        stringstream value;
        value << _counters[i];

        stringstream key;
        key << COUNTER << i;

        state[key.str()] = value.str();
    }//for
    trans->send(state);

    LOG("End TrArrCounterMB %d:", _id);
    LOG("--------------------");

    return p;
}

int TrArrCounterMB::configure(Vector<String> &conf, ErrorHandler *errh) {
    IntArg parser;
    int size;

    parser.parse(conf[0], _id);

    parser.parse(conf[1], size);

    for (int i = 0; i < size; ++i) {
        _counters.push_back(INIT_COUNTER);
    }//for

    LOG("Counter MB id is %d!\n", _id);

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TrArrCounterMB)