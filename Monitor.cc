#include "FTStateElement.hh"
#include "FTAppenderElement.hh"
#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "Monitor.hh"

CLICK_DECLS

Monitor::Monitor() : _counter(INIT_COUNTER) {};

Monitor::~Monitor() {};

Packet *Monitor::simple_action(Packet *p) {
    LOG("--------------------");
    std::cout << "Begin Monitor :" << _id << std::endl;
    Router *r = this->router();

//    for (int i = 0; i < r->elements().size(); ++i) {
//        LOG("%d-Element name is: %s", i, r->ename(i).c_str());
//    }//for

    //TODO: change the name of stateElement back to se
    FTStateElement *stateElement = (FTStateElement *)(r->find("se"));
//    stringstream ess;
//    ess << "se" << (int)_id;
//    DEBUG("state element: %s", ess.str().c_str());
//    FTStateElement *stateElement = (FTStateElement *)(r->find(ess.str().c_str()));

    //Getting the state's value from the FTStateElement
    stringstream ss;
    string value;

    std::cout << "Begin Monitor1 :" << _id << std::endl;
    if (stateElement->getPrimaryState(COUNTER, value)) {
	std::cout << "before in here" << std::endl;
        if (value.size() != 0) {
	    std::cout << "in here" << std::endl;
            ss << value;
            ss >> _counter;
        }//if
	std::cout << "out here" << std::endl;
    }//if

    std::cout << "Begin Monitor2 :" << _id << std::endl;
    stringstream ss2;
    ss2 << (_counter + 1);
    ss2 >> value;

    stateElement->putPrimaryState(COUNTER, value);

    std::cout << "Begin Monitor3 :" << _id << std::endl;
    _counter++;

    LOG("Packet id is: %llu", FTAppenderElement::getPacketId(p));
    LOG("Packet counter read is %d, and written %d", _counter - 1, _counter);

    LOG("End Monitor %d:", _id);
    LOG("--------------------");

    return p;
}

int Monitor::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX).complete() < 0)
        return -1;

    LOG("Monitor id is %d!\n", _id);

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Monitor)

