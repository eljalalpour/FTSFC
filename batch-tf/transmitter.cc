#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "transmitter.hh"

CLICK_DECLS

Transmitter::Transmitter() {
    Util _util;
    _util.init(inoperation);
};

Transmitter::~Transmitter() { };

void Transmitter::_print_ip_port_list() {
#ifndef DEBUG
    return;
#else
    for (size_t i = 0; i < _ips.size(); ++i) {
        DEBUG("%s:%u", _ips[i].c_str(), _ports[i]);
    }//for
#endif
}

int Transmitter::configure(Vector<String> &conf, ErrorHandler *) {

    BoundedIntArg b_int(0, 0xFFFF);
    for (int i = 0; i < conf.size(); ++i) {
        string line(conf[i].c_str());
        vector<string> strs;
        _split(line, ':', strs);
        string ip = strs[0];
        uint16_t port;
        b_int.parse(String(strs[1].c_str()), port);

        _ips.push_back(ip);
        _ports.push_back(port);
    }//for
    _client.set_ip_ports(_ips, _ports);
    _print_ip_port_list();

    return 0;
}


Packet *Transmitter::simple_action(Packet *p) {
    return p;
}

bool Transmitter::send() {
    return _client.send(inoperation);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Transmitter)