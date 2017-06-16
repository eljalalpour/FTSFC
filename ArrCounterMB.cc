#include "FTStateElement.hh"
#include "FTAppenderElement.hh"
#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "ArrCounterMB.hh"

CLICK_DECLS

ArrCounterMB::ArrCounterMB() {};

ArrCounterMB::~ArrCounterMB() {};

Packet *ArrCounterMB::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin ArrCounterMB %d:", _id);
    Router *r = this->router();

    //TODO: change the name of stateElement back to se
    FTStateElement *stateElement = (FTStateElement *)(r->find("se"));
//    stringstream ess;
//    ess << "se" << (int)_id;
//    DEBUG("state element: %s", ess.str().c_str());
//    FTStateElement *stateElement = (FTStateElement *)(r->find(ess.str().c_str()));

    for (int i = 0; i < _counters.size(); ++i) {
        stringstream value;
        value << _counters[i];

        stringstream key;
        key << COUNTER << i;

        stateElement->putPrimaryState(key.str(), value.str());
    }//for

    LOG("End ArrCounterMB %d:", _id);
    LOG("--------------------");

    return p;
}

int ArrCounterMB::configure(Vector<String> &conf, ErrorHandler *errh) {
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
EXPORT_ELEMENT(ArrCounterMB)