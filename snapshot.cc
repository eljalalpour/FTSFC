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
                .complete() < 0)
        return -1;

    LOG("Snapshot period is %d ms, delay is %d ms!\n",
        _period, _delay);

    // parameters are in mili/micro-seconds,
    // convert them to nano-seconds
    _period *= MS2NS;
    _delay *= MS2NS;
    _first_packet_seen = false;

    return 0;
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

    DEBUG("End Snapshot");
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Snapshot)
