#include "FTStateElement.hh"
#include "FTAppenderElement.hh"
#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "CounterMB.hh"

CLICK_DECLS

CounterMB::CounterMB() : _counter(INIT_COUNTER) {};

CounterMB::~CounterMB() {};

Packet *CounterMB::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin CounterMB %d:", _id);
    Router *r = this->router();

//    for (int i = 0; i < r->elements().size(); ++i) {
//        LOG("%d-Element name is: %s", i, r->ename(i).c_str());
//    }//for

    //TODO: change the name of stateElement back to se
//    FTStateElement *stateElement = (FTStateElement *)(r->find("se"));
    stringstream ess;
    ess << "se" << (int)_id;
    DEBUG("state element: %s", ess.str().c_str());
    FTStateElement *stateElement = (FTStateElement *)(r->find(ess.str().c_str()));

    //Getting the state's value from the FTStateElement
    stringstream ss;
    string value;

    if (stateElement->getPrimaryState(COUNTER, value)) {
        if (value.size() != 0) {
            ss << value;
            ss >> _counter;
        }//if
    }//if

    stringstream ss2;
    ss2 << (_counter + 1);
    ss2 >> value;

    stateElement->putPrimaryState(COUNTER, value);

    _counter++;

    LOG("Packet id is: %llu", FTAppenderElement::getPacketId(p));
    LOG("Packet counter read is %d, and written %d", _counter - 1, _counter);

    LOG("End CounterMB %d:", _id);
    LOG("--------------------");

    return p;
}

int CounterMB::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX).complete() < 0)
        return -1;

    LOG("Counter MB id is %d!\n", _id);

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CounterMB)