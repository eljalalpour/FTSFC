#include <click/config.h>
#include <click/router.hh>
#include "appender.hh"

CLICK_DECLS

Appender::Appender () { };

Appender::~Appender() { };

Packet *Appender::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Appender with index %d:", _index);


    DEBUG("End Appender %d:", _index);
    DEBUG("--------------------");

    return p;
}

void Appender::reserve_state_space(Packet* p) {

}

CLICK_ENDDECLS
EXPORT_ELEMENT(Appender)
