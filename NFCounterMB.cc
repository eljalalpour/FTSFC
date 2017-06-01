#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "FTAppenderElement.hh"
#include "NFCounterMB.hh"

CLICK_DECLS

NFCounterMB::NFCounterMB() : _counter(INIT_COUNTER) {};

NFCounterMB::~NFCounterMB() {};

Packet *NFCounterMB::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin NFCounterMB %d:", _id);

    _counter++;
    click_chatter("1");
    WritablePacket* q1 = p->uniqueify();
    WritablePacket* q2 = p->uniqueify();
    WritablePacket* q3 = p->uniqueify();
    WritablePacket* q4 = p->uniqueify();

    click_chatter("2");
    RandomState rs;
    FTPiggyBackMessage msg1 = rs.random_message(2, 1);
    click_chatter("3");
    WritablePacket* q5 = FTAppenderElement::encodeStates(p, msg1);
    WritablePacket* q6 = FTAppenderElement::encodeStates(p, msg1);
    WritablePacket* q7 = FTAppenderElement::encodeStates(p, msg1);
    click_chatter("4");

    FTPiggyBackMessage msg2, msg3, msg4;
    click_chatter("5");
    FTAppenderElement::decodeStates(q5, msg2);
    click_chatter("51");
    FTAppenderElement::decodeStates(q6, msg3);
    click_chatter("52");
    FTAppenderElement::decodeStates(q7, msg4);
    click_chatter("6");

    LOG("End NFCounterMB %d:", _id);
    LOG("--------------------");

    return p;
}

int NFCounterMB::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX).complete() < 0)
        return -1;

    LOG("NFCounterMB id is %d!\n", _id);

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NFCounterMB)