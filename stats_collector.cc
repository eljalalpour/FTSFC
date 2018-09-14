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
    auto _shared_state_elm = (SharedLockFreeState *)(r->find("shared_state"));
    _log_table_stats.push_back(_shared_state_elm->log_table_length());

    for (auto i = 0; i < _buffer; ++i) {
        String buffer_name = "b" + String(i) + "/";
        LOG("Finding %s!", buffer_name.c_str());
        auto _buffer_elm = (Buffer *) (r->find("buffer", buffer_name));
        _buffer_stats[i].push_back(_buffer_elm->length());
    }

}

CLICK_ENDDECLS
EXPORT_ELEMENT(StatsCollector)
