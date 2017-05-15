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
    click_chatter("finding state element");
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

    click_chatter("Packet id is: %d", FTAppenderElement::getPacketId(p));
    click_chatter("Packet counter is: %d", _counter);

    stringstream ss2;
    ss2 << (_counter + 1);
    ss2 >> value;

    stateElement->putPrimaryState(COUNTER, value);

    _counter++;

    return p;
}

int CounterMB::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX).complete() < 0)
        return -1;

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CounterMB)