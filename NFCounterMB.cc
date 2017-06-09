#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include <iostream>
#include <fstream>
#include "FTAppenderElement.hh"
#include "NFCounterMB.hh"

using namespace std;

CLICK_DECLS

NFCounterMB* _counter_mb;

void signal_handler2(int signal) {
    DEBUG("Interrupt signal (%d) received. Writing packets timestamps to file!", signal);
    click_chatter("In signal %d handler", signal);
    cout << "Counter is: " << _counter_mb->counter() << endl;
    _counter_mb->write_to_file();
    exit(signal);
}

NFCounterMB::NFCounterMB() : _counter(INIT_COUNTER) { _counter_mb = this; };

NFCounterMB::~NFCounterMB() {};

unsigned int NFCounterMB::counter() {
    return _counter;
}

Packet *NFCounterMB::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin NFCounterMB %d:", _id);

    _counter++;
    
//    WritablePacket* q1 = p->uniqueify();
//    WritablePacket* q2 = p->uniqueify();
//    WritablePacket* q3 = p->uniqueify();
//    WritablePacket* q4 = p->uniqueify();

//    RandomState rs;
//    FTPiggyBackMessage msg1 = rs.random_message(2, 1);
//    WritablePacket* q5 = FTAppenderElement::encodeStates(q1, msg1);
//    WritablePacket* q6 = FTAppenderElement::encodeStates(q2, msg1);
//    WritablePacket* q7 = FTAppenderElement::encodeStates(q3, msg1);
//
//    FTPiggyBackMessage msg2, msg3, msg4;
//    WritablePacket* q8  = FTAppenderElement::encodeStates(q1, msg1);
//    WritablePacket* q9  = FTAppenderElement::encodeStates(q2, msg1);

    LOG("End NFCounterMB %d:", _id);
    LOG("--------------------");

    return p;
}

int NFCounterMB::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX).complete() < 0)
        return -1;

    LOG("NFCounterMB id is %d!\n", _id);

    signal(SIGINT, signal_handler2);
    signal(SIGTERM, signal_handler2);

    return 0;
}

void NFCounterMB::write_to_file() {
    std::ofstream ofs("counter.txt", std::ofstream::out | std::ofstream::app);
    ofs << _counter << "\n";
    ofs.close();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFCounterMB)