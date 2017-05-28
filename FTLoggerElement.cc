#include "FTLoggerElement.hh"
#include <click/config.h>
#include <click/args.hh>
#include <fstream>
#include "FTAppenderElement.hh"

CLICK_DECLS

FTLoggerElement* logger;

void signal_handler(int signal) {
    LOG("Interrupt signal (%d) received. Writing packets timestamps to file!", signal);
    logger->write_to_file();
    exit(signal);
}

FTLoggerElement::FTLoggerElement() {
    logger = this;
    _file_created = false;
    _count = std::numeric_limits<double>::infinity();
}

FTLoggerElement::~FTLoggerElement() { }

int FTLoggerElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    StringArg parser;
    parser.parse(conf[0], _log_file);

    if (conf.size() > 1) {
        IntArg intParser;
        intParser.parse(conf[1], _count);
    }

    signal(SIGINT, signal_handler);

    return 0;
}

void FTLoggerElement::write_to_file() {
    std::ofstream ofs(_log_file.c_str(), std::ofstream::out | std::ofstream::app);

    if (!_file_created) {
        ofs << "id,timestamp\n";
        _file_created = true;
    }//if

    for (auto it = _pkt_time.begin(); it != _pkt_time.end(); ++it) {
        ofs << it->first << "," << it->second << std::endl;
    }//for
    ofs.close();
}

void FTLoggerElement::push(int port, Packet *p) {
    LOG("------------------------------");
    LOG("Begin FTLoggerElement");

    _pkt_time[FTAppenderElement::getPacketId(p)] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    if (_pkt_time.size() >= _count) {
        write_to_file();
        _pkt_time.clear();
    }//if

    output(0).push(p);

    LOG("End FTLoggerElement");
    LOG("------------------------------");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTLoggerElement)