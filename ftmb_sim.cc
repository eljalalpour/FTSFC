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
                .read("SHIFT", _shift)
                .complete() < 0)
        return -1;

    // Config parameters are in ms, convert them to us
    _period *= MS2US;
    _delay *= MS2US;
    _first_packet_seen = false;

    DEBUG("FTMBSim period is %d, delay is %d!\n", _period, _delay);

    return 0;
}

Packet *FTMBSim::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMBSim");

    if (!_first_packet_seen) {
        _first_packet_seen = true;
        _last_snapshot_timestamp = Timestamp::now();

        // Sleep for shift
        auto random_sleep = _shift * MS2US;
        usleep(random_sleep);
    }//if

    auto now = Timestamp::now();
    auto passed = (now - _last_snapshot_timestamp).usec();
    if (passed >= _period) {
        usleep(_delay);
        _last_snapshot_timestamp = now;
    }//if

    Router *r = this->router();

    LockFreeArray *lfc = (LockFreeArray *)(r->find("array"));
    ++lfc->array[0];

    DEBUG("End FTMBSim");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBSim)
