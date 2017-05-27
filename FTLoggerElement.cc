#include <click/config.h>
#include <click/args.hh>
#include <fstream>
#include "FTLoggerElement.hh"
#include <limits>

CLICK_DECLS

FTLoggerElement::FTLoggerElement() { }

FTLoggerElement::~FTLoggerElement() {
    _write_to_file();
}

int FTLoggerElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
                .read(LOG_PATH_CONF, _path)
                .complete() < 0)
        return -1;

    if (Args(conf, this, errh)
                .read(COUNT_CONF, _count).complete() < 0) {
        _count = std::numeric_limits<double>::infinity();
    }//if


    //LOG("Configuration of state element: id: %d, vlanId: %d, f: %d\n", _id, _vlanId, _failureCount);
    signal(SIGINT, FTLoggerElement::_signal_handler);

    return 0;
}

void FTLoggerElement::_write_to_file() {
    std::ofstream ofs(_path.c_str(), std::ofstream::out | std::ofstream::app);
    for (auto it = _pkt_time.begin(); it != _pkt_time.end(); ++it) {
        ofs << it->first << " " << it->second << std::endl;
    }//for
    ofs.close();
}

void FTLoggerElement::_signal_handler(int signal) {
    click_chatter("Interrupt signal (%d) received. Writing packets timestamps to file!", signal);
    FTLoggerElement::_write_to_file();
}


void FTLoggerElement::push(int port, Packet *p) {
    LOG("------------------------------");
    LOG("Begin FTLoggerElement");

    _pkt_time[FTAppenderElement::getPacketId(p)] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    if (_pkt_time.size() >= _count) {
        _write_to_file();
        _pkt_time.clear();
    }//if

    output(0).push(p);

    LOG("End FTLoggerElement");
    LOG("------------------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTLoggerElement)