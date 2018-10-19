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
                .read("FIRST_DELAY", _first_delay)
                .complete() < 0)
        return -1;

    // Config parameters are in ms, convert them to us
    _period *= MS2US;
    _delay *= MS2US;
    _first_delay *= MS2US;
    _first_packet_seen = false;

    DEBUG("FTMBSim period is %d, delay is %d!\n", _period, _delay);

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

        if (_first_delay == -1) {
            _first_delay = click_random() % MAX_FIRST_DELAY_MS;
        }//if
    }//if
    else {
        _first_delay = 0;
    }//else

    auto passed = (Timestamp::now() - _last_snapshot_timestamp).usec();
    if (passed >= _period + _first_delay) {
        usleep(_delay);
        _last_snapshot_timestamp = Timestamp::now();
    }//if

    _init_shared_state();
    ++_shared_state->array[0];

    DEBUG("End FTMBSim");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBSim)
