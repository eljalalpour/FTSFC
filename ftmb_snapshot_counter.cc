#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ftmb_snapshot_counter.hh"

CLICK_DECLS

FTMBSnapshotCounter::FTMBSnapshotCounter () : _counter(0) { };

FTMBSnapshotCounter::~FTMBSnapshotCounter() { };

void FTMBSnapshotCounter::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (FTMBSharedStateCounter *)(r->find(_shared_state_element_name));
}

int FTMBSnapshotCounter::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("SHARED_STATE", _shared_state_element_name)
                .read("PERIOD", _period)
                .read("DELAY", _delay)
                .complete() < 0)
        return -1;

    LOG("FTMBSnapshotCounter index is %d!\n", _index);
    LOG("Snapshot period is %d ms, delay is %d ms!\n",
        _period, _delay);

    // parameters are in mili/micro-seconds,
    // convert them to nano-seconds
    _period *= MS2NS;
    _delay *= MS2NS;
    _first_packet_seen = false;

    _init_shared_state();

    return 0;
}

Packet *FTMBSnapshotCounter::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTMBSnapshotCounter with index %d:", _index);

    if (!_first_packet_seen) {
        _first_packet_seen = true;
        _last_snapshot_timestamp = CLOCK_NOW;
    }//if

    if (Util::npassed(_last_snapshot_timestamp, _period)) {
        Util::nsleep(_delay);
        _last_snapshot_timestamp = CLOCK_NOW;
    }//if

    Locker* locker = _shared_state->preprocess(_index, _index);
    _counter ++;
    _shared_state->postprocess(_index, locker, p, &output(0));

    DEBUG("End FTMBSnapshotCounter %d:", _index);
    DEBUG("--------------------");

//    return p;
    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBSnapshotCounter)