#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "NFCounterMB.hh"

CLICK_DECLS

NFCounterMB::NFCounterMB() : _counter(INIT_COUNTER) {};

NFCounterMB::~NFCounterMB() {};

Packet *NFCounterMB::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin NFCounterMB %d:", _id);

    _counter++;

    WritablePacket* q1 = p->uniqueify();
    WritablePacket* q2 = p->uniqueify();
    WritablePacket* q3 = p->uniqueify();
    WritablePacket* q4 = p->uniqueify();
    WritablePacket* q5 = p->uniqueify();
    WritablePacket* q6 = p->uniqueify();

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