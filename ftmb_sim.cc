#include "lock_free_array.hh"
#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "nf_lock_free_counter.hh"

CLICK_DECLS

FTMBSim::FTMBSim () { };

FTMBSim::~FTMBSim() { };

int FTMBSim::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    if (Args(conf, this, errh)
                .read("PERIOD", _period)
                .read("DELAY", _delay)
                .complete() < 0)
        return -1;
    LOG("FTMBSim period is %d, delay is %d!\n", _period, _delay);

    return 0;
}

Packet *FTMBSim::simple_action(Packet *p) {
    LOG("--------------------");
    LOG("Begin FTMBSim with index %d:", _index);

    // TODO: implement periodic delay

    LOG("End FTMBSim %d:", _index);
    LOG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTMBSim)
