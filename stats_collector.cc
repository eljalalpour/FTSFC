#include <click/config.h>
#include <click/args.hh>
#include <click/router.hh>
#include <sstream>
#include "buffer.hh"
#include "shared_lock_free_state.hh"
#include "stats_collector.hh"

CLICK_DECLS

StatsCollector* logger;

void signal_handler(int signal) {
    LOG("Interrupt signal (%d) received. Writing packets timestamps to file!", signal);

    logger->write_to_file();
    exit(signal);
}

StatsCollector::StatsCollector () {
    logger = this;
};

StatsCollector::~StatsCollector() { };

int StatsCollector::configure(Vector<String> &conf, ErrorHandler *errh) {
    // set id and f params
    if (Args(conf, this, errh)
                .read("BUFFER", _buffer)
                .read("STATE", _state)
                .read("PATH", _path)
                .read("PERIOD", _period)
                .complete() < 0)
        return -1;

    if (_buffer > 0) {
        for (auto i = 0; i < _buffer; ++i) {
            Vector<size_t> elem;
            _buffer_stats.push_back(elem);
        }//for
    }//if

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    _tt = std::thread(StatsCollector::_run, this);

    return 0;
}

Packet *StatsCollector::simple_action(Packet *p) {
    return p;
}

void StatsCollector::write_to_file() {
    std::ofstream ofs(_path.c_str(), std::ofstream::out);

    for (int i = 0; i < _log_table_stats.size(); ++i) {
        ofs << _log_table_stats[i];
        for (int j = 0; j < _buffer; ++j)
            ofs << "," << _buffer_stats[j][i];
        ofs << std::endl;
    }//for
    ofs.close();

    LOG("Wrote stats into '%s'!", _path.c_str());
}

void StatsCollector::collect() {
    Router *r = this->router();

    if (_state > 0) {
        auto _shared_state_elm = (SharedLockFreeState *) (r->find(SHARED_STATE));
        _log_table_stats.push_back(_shared_state_elm->log_table_length());
    }//if
    else {
        _log_table_stats.push_back(INVALID_LOG);
    }//else

    if (_buffer == 1) {
        try {
            auto _buffer_elm = (Buffer *) (r->find(BUFFER));
            _buffer_stats[0].push_back(_buffer_elm->length());
        }//try
        catch(...) {
            LOG("%s not found, searching for %s", BUFFER, (FT_BLOCK_PREFIX + String(1) + "/").c_str());
        }//catch
    }//if
    for (auto i = 0; i < _buffer && _buffer > 1; ++i) {
        String block = FT_BLOCK_PREFIX + String(i + 1) + "/";
        auto _buffer_elm = (Buffer *) (r->find(BUFFER, block));
        _buffer_stats[i].push_back(_buffer_elm->length());
    }

}

CLICK_ENDDECLS
EXPORT_ELEMENT(StatsCollector)
