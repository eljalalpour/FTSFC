#include "shared_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ftmb_ol.hh"

CLICK_DECLS

FTMBOutputLogger::FTMBOutputLogger () { };

FTMBOutputLogger::~FTMBOutputLogger() { };

int FTMBOutputLogger::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("PER_PACKET", _per_packet_latency)
                .complete() < 0)
        return -1;

    LOG("FTMBOutputLogger per packet latency is %d us!\n",
        _per_packet_latency);

    // parameters are in mili/micro-seconds,
    // convert them to nano-seconds
    _per_packet_latency *= US2NS;
    _loop_count = Util::find_dummy_loop_count(_per_packet_latency);

    LOG("Loop count found: %d", _loop_count);

    return 0;
}

Packet *FTMBOutputLogger::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMBOutputLogger");

    // Artificial latency
    Util::dummy_loop(_loop_count);

    DEBUG("End FTMBOutputLogger");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBOutputLogger)
