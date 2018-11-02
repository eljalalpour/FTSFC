#include "lock_free_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ftmb_sim.hh"

CLICK_DECLS

FTMBSim::FTMBSim () { };

FTMBSim::~FTMBSim() { };

int FTMBSim::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("PERIOD", _period)
                .read("DELAY", _delay)
                .read("PER_PACKET", _per_packet_latency)
                .complete() < 0)
        return -1;

    LOG("FTMBSim period is %d ms, delay is %d ms, per packet latency is %d us!\n",
        _period, _delay, _per_packet_latency);

    // parameters are in mili/micro-seconds,
    // convert them to nano-seconds
    _period *= MS2NS;
    _delay *= MS2NS;
    _per_packet_latency *= US2NS;
    _first_packet_seen = false;

    _loop_count = Util::find_dummy_loop_count(_per_packet_latency);

//    LOG("Loop count found: %d", _loop_count);

    return 0;
}

void FTMBSim::_init_shared_state() {
    if (!_init_state) {
        Router *r = this->router();
        _shared_state = (LockFreeArray *)(r->find("array"));
        _init_state = true;
    }//if
}

Packet *FTMBSim::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMBSim");

    if (!_first_packet_seen) {
        _first_packet_seen = true;
        _last_snapshot_timestamp = CLOCK_NOW;
    }//if

    if (Util::npassed(_last_snapshot_timestamp, _period)) {
        Util::nsleep(_delay);
        _last_snapshot_timestamp = CLOCK_NOW;
    }//if

//    Util::nsleep(_per_packet_latency);
    Util::dummy_loop(_loop_count);

//    _init_shared_state();
    //TODO: adding the following line results in Segmentation fault, fix it
    //_shared_state->increment(0);

    DEBUG("End FTMBSim");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBSim)
