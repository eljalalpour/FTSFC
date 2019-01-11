#include "shared_state.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ftmb_master.hh"

CLICK_DECLS

FTMBMaster::FTMBMaster () { };

FTMBMaster::~FTMBMaster() { };

int FTMBMaster::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("PERIOD", _period)
                .read("DELAY", _delay)
                .read("PER_PACKET", _per_packet_latency)
                .read("SHARED_ARRAY", _shared_array_element_name)
                .complete() < 0)
        return -1;

    LOG("FTMBMaster period is %d ms, delay is %d ms, per packet latency is %d us!\n",
        _period, _delay, _per_packet_latency);

    // parameters are in mili/micro-seconds,
    // convert them to nano-seconds
    _period *= MS2NS;
    _delay *= MS2NS;
    _per_packet_latency *= US2NS;
    _first_packet_seen = false;

    _loop_count = Util::find_dummy_loop_count(_per_packet_latency);

    LOG("Loop count found: %d", _loop_count);

    _init_shared_state();

    return 0;
}

void FTMBMaster::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (SharedState *)(r->find(_shared_array_element_name));
}

Packet *FTMBMaster::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMBMaster");

    if (!_first_packet_seen) {
        _first_packet_seen = true;
        _last_snapshot_timestamp = CLOCK_NOW;
    }//if

    if (Util::npassed(_last_snapshot_timestamp, _period)) {
        Util::nsleep(_delay);
        _last_snapshot_timestamp = CLOCK_NOW;
    }//if

    Util::dummy_loop(_loop_count);


    //TODO: adding the following line results in Segmentation fault, fix it
    //_shared_state->increment(0);

    DEBUG("End FTMBMaster");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBMaster)
