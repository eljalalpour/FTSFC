#include <click/config.h>
#include <click/router.hh>
#include <click/args.hh>
#include "ft_tran.hh"

CLICK_DECLS

FTTran::FTTran () {
    _runs = 0;
    _aborts = 0;
    _packets = 0;
}

FTTran::~FTTran() { }

void FTTran::_init_shared_state() {
    Router *r = this->router();
    _shared_state = (FTSharedState *)(r->find("shared_state"));
    _shared_locks = (SharedLocks *)(r->find("share_locks"));
}

void FTTran::_init_times(int conflict_prob) {
    _lock_time = TOTAL_TIME * conflict_prob * 10;
    _processing_time = TOTAL_TIME * (100 - conflict_prob) * 10;

    DEBUG("FTTran lock time is %d!\n", _lock_time);
    DEBUG("FTTran processing time is %d!\n", _processing_time);
}

int FTTran::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set index param
    int conflict_prob; // between 1 to 99
    if (Args(conf, this, errh)
                .read("INDEX", _index)
                .read("CONFLICT_PROB", conflict_prob)
                .complete() < 0)
        return -1;

    DEBUG("FTTran index is %d!\n", _index);
    DEBUG("FTTran conflict probability is %d!\n", conflict_prob);

    _init_shared_state();
    _init_times(conflict_prob);

    return 0;
}

String FTTran::read_param(Element *e, void *thunk)
{
    FTTran *td = (ToDevice *)e;

    String re = String(td->_runs);
    re.append(",");
    re.append(String(td->_aborts));
    re.append(",");
    re.append(String(td->_packets));

    return re;
}

Packet *FTTran::simple_action(Packet *p) {
    DEBUG("--------------------");
    DEBUG("Begin FTTran with index %d:", _index);

    _packets += 1;

    int first = 0;
    int second = 1;

    if (_index % 2 == 1) {
        first = 1;
        second = 0;
    }//if

    BEGIN:
    /// Track the number of transactions
    _runs += 1;

    /// Acquire lock for first
    _shared_locks->lock(first);

    /// Lock time: Wait for some time
    Util::nsleep(_lock_time);

    /// Acquire lock for second
    if (_shared_locks->try_lock(second)) {
        /// if successful, wait for some time
        Util::nsleep(_processing_time);
    }//if
    else {
        /// Abort transaction
        /// Track the number of aborts
        _aborts += 1;
        _shared_locks->unlock(first);

        /// Restart transaction
        goto BEGIN;
    }//else

    /// Release locks
    _shared_locks->unlock(second);
    _shared_locks->unlock(first);


    DEBUG("End FTTran %d:", _index);
    DEBUG("--------------------");

    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTTran)
