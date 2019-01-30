#include "shared_state.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "snapshot.hh"

CLICK_DECLS

Snapshot::Snapshot () { };

Snapshot::~Snapshot() { };

int Snapshot::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("PERIOD", _period)
                .read("DELAY", _delay)
                .read("PER_PACKET", _per_packet_latency)
                .complete() < 0)
        return -1;

    LOG("Snapshot period is %d ms, delay is %d ms, per packet latency is %d us!\n",
        _period, _delay, _per_packet_latency);

    // parameters are in mili/micro-seconds,
    // convert them to nano-seconds
    _period *= MS2NS;
    _delay *= MS2NS;
    _per_packet_latency *= US2NS;
    _first_packet_seen = false;

    _loop_count = Util::find_dummy_loop_count(_per_packet_latency);
    _init_shared_state();

    return 0;
}

void Snapshot::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (SharedState *)(r->find("array"));
}

Packet *Snapshot::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin Snapshot");

    if (!_first_packet_seen) {
        _first_packet_seen = true;
        _last_snapshot_timestamp = CLOCK_NOW;
    }//if

    if (Util::npassed(_last_snapshot_timestamp, _period)) {
        Util::nsleep(_delay);
        _last_snapshot_timestamp = CLOCK_NOW;
    }//if

    Util::dummy_loop(_loop_count);

    DEBUG("End Snapshot");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Snapshot)
