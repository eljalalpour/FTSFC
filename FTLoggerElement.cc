#include "FTLoggerElement.hh"
#include <click/config.h>
#include <click/args.hh>
#include <fstream>
#include <csignal>
#include <click/router.hh>
#include "CounterMB.hh"
#include "FTAppenderElement.hh"

CLICK_DECLS

FTLoggerElement* logger;

void signal_handler(int signal) {
    DEBUG("Interrupt signal (%d) received. Writing packets timestamps to file!", signal);
    DEBUG("Writing %llu packets", logger->num_packets());
    click_chatter("In signal %d handler", signal);
    logger->write_to_file();
    ofstream ofile("counter.txt");
    Router* r = logger->router();
    CounterMB* mb = (CounterMB*)(r->find("cnt"));
    ofile << mb->_couter;
    ofile.close();
    exit(signal);
}

FTLoggerElement::FTLoggerElement() {
    _file_created = false;
    _count = COUNT_DEF_VAL;
    _ip_offset = IP_OFFSET_DEF_VAL;

    logger = this;
}

FTLoggerElement::~FTLoggerElement() { }

int FTLoggerElement::configure(Vector<String> &conf, ErrorHandler *errh) {
    StringArg parser;
    parser.parse(conf[0], _log_file);

    if (conf.size() > 1) {
        IntArg intParser;
        intParser.parse(conf[1], _ip_offset);
    }//if
    if (conf.size() > 2) {
        IntArg intParser;
        intParser.parse(conf[2], _count);
    }//if

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

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

Packet* FTLoggerElement::simple_action(Packet* p) {
    LOG("------------------------------");
    LOG("Begin FTLoggerElement");

    auto id = FTAppenderElement::getPacketId(p);
    _pkt_time[id] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    if (_pkt_time.size() >= _count) {
        write_to_file();
        _pkt_time.clear();
    }//if

    LOG("End FTLoggerElement");
    LOG("------------------------------");
    return p;
}

size_t FTLoggerElement::num_packets() {
    return _pkt_time.size();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTLoggerElement)