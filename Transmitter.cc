#include "FTStateElement.hh"
#include "FTAppenderElement.hh"
#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "Transmitter.hh"

CLICK_DECLS

Transmitter::Transmitter() : _counter(INIT_COUNTER) {};

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

void Transmitter::_split(string& str, char dlm, vector<string>& tokens) {
    int last_index = 0;
    int i = 0;
    for (; i < str.size(); ++i) {
        if (str[i] == dlm && i - last_index > 0) {
            string token(str, last_index, i - last_index);
            tokens.push_back(token);
            last_index = i + 1;
        }//if
    }//for

    if (i - 1 - last_index > 0) {
        string token(str, last_index, i - 1 - last_index);
        tokens.push_back(token);
        last_index = i + 1;
    }//if
}

int Transmitter::configure(Vector<String> &conf, ErrorHandler *errh) {
    Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX);

    BoundedIntArg b_int(0, 0xFFFF);
    for (int i = 1; i < conf.size(); ++i) {
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

bool Transmitter::send(FTTimestampState& ts_state) {
    return _client.send(ts_state);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Transmitter)