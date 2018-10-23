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

    // Config parameters are in ms, convert them to us
    _period *= MS2US;
    _delay *= MS2US;
    _first_packet_seen = false;

    LOG("FTMBSim period is %d us, delay is %d us, per packet latency is %d us!\n",
            _period, _delay, _per_packet_latency);

    _delay *= US2NS;
    _per_packet_latency *= US2NS;

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
        _last_snapshot_timestamp = Timestamp::now();
    }//if

    auto passed = (Timestamp::now() - _last_snapshot_timestamp).usec();
    if (passed >= _period) {
        _last_snapshot_timestamp = Timestamp::now();
        Util::nsleep(_delay);
    }//if

    Util::nsleep(_per_packet_latency);

    _init_shared_state();
    ++_shared_state->array[0];

    DEBUG("End FTMBSim");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBSim)
