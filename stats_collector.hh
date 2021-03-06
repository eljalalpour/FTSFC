#pragma once

#include "defs.hh"
#include <click/config.h>
#include <click/element.hh>
#include <fstream>
#include <csignal>
#include <thread>
#include <chrono>

CLICK_DECLS

#define FT_BLOCK_PREFIX "b"
#define INVALID_LOG 0
#define SHARED_STATE "shared_state"
#define BUFFER "buffer"

class StatsCollector : public Element {
private:
    int _buffer;
    int _state;
    String _path;
    int _period;
    std::thread _tt;

    Vector<Vector<size_t>> _buffer_stats;
    Vector<size_t> _log_table_stats;

    static void _run(StatsCollector* state_coll) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(state_coll->_period));
            state_coll->collect();
        }//while
    }

public:
    StatsCollector ();

    ~StatsCollector();

    const char *class_name() const { return "StatsCollector"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *);

    int configure(Vector<String> &, ErrorHandler *);

    void write_to_file();

    void collect();
};

CLICK_ENDDECLS

