#include "lock_free_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "nf_lock_free_counter.hh"

CLICK_DECLS

Appender::Appender () { };

Appender::~Appender() { };

Packet *Appender::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin Appender with index %d:", _index);
    Router *r = this->router();

    LockFreeArray *lfc = (LockFreeArray *)(r->find("array"));
    ++lfc->counters[_index];

    LOG("End Appender %d:", _index);
    LOG("--------------------");

    return p;
}

void Appender::reserve_state_space(Packet* p) {

}

CLICK_ENDDECLS
EXPORT_ELEMENT(Appender)
