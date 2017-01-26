#include "FTStateElement.hh"

#include <click/config.h>
#include "CounterMB.hh"
#include <click/router.hh>
#include <clicknet/tcp.h>

CLICK_DECLS


CounterMB::CounterMB() : _counter(INIT_COUNTER) {};

CounterMB::~CounterMB() {};

Packet *CounterMB::simple_action(Packet *p) {
//    click_chatter("--------------------");
//    click_chatter("In Counter MB");
//    click_chatter("finding state element");
    Router *r = this->router();

//    for (int i = 0; i < r->elements().size(); ++i) {
//        click_chatter("%d-Element name is: %s", i, r->ename(i).c_str());
//    }//for

    FTStateElement *stateElement = (FTStateElement *)(r->find("se"));

    //Getting the state's value from the FTStateElement
    stringstream ss;
    string value;

    if (stateElement->getPrimaryState(COUNTER, value)) {
        if (value.size() != 0) {
            ss << value;
            ss >> _counter;
        }//if
    }//if

//    click_chatter("Packet counter is: %d", _counter);

    stringstream ss2;
    ss2 << (_counter + 1);
    ss2 >> value;

    stateElement->putPrimaryState(COUNTER, value);

    _counter++;

//    click_chatter("--------------------");
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CounterMB)