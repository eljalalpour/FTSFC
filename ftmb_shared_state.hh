#pragma once
#include <deque>
#include "shared_state_base.hh"

using std::deque;

class FTMBSharedState: SharedStateBase {
public:
    FTMBSharedState ();

    ~FTMBSharedState();

    const char *class_name() const { return "FTMBSharedState"; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

private:
    int _queues;
    VectorOfClocks _maxs; /// MAX in FTMB's definition
    std::deque<Locker> _maxs_lockers; /// Locks for _maxs
    std::deque<VectorOfClocks> _counters; /// counters for shared variables
};
