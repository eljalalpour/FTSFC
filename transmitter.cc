#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include <click/ipaddress.hh>
#include "transmitter.hh"

CLICK_DECLS

Transmitter::Transmitter() {
    memset(inoperations, 0, sizeof(TFStates));
};

Transmitter::~Transmitter() { };

void Transmitter::_print_ip_port_list(vector<string>& ips, vector<uint16_t>& ports) {
    for (size_t i = 0; i < ips.size(); ++i) {
        LOG("%s:%u", ips[i].c_str(), ports[i]);
    }//for
}

int Transmitter::configure(Vector<String> &conf, ErrorHandler * errh) {
    if (Args(conf, this, errh)
                .read("QUEUES", _queues)
                .read("BATCH", _batch)
                .consume() < 0)
        return -1;
    LOG("Queues: %d", _queues);
    
    /// Read ips
    IPAddress ip;
    for (int i = 0; i < conf.size(); ++i) {
        if (!IPAddressArg::parse(conf[i], ip))
	        continue;

        _ips.emplace_back(conf[i].c_str());
    }//for

    /// For each queue, we need a client
    /// communicating with the same port number across all given replica IPs.
    for (int i = 0; i < _queues; ++i) {
        vector<uint16_t> ports(_ips.size(), MIN_PORT + i);

        LOG("For queue %d", i);
        _print_ip_port_list(_ips, ports);
        _clients[i].set_ip_ports(_ips, ports, _batch);
    }//for
    
    return 0;
}

Packet *Transmitter::simple_action(Packet *p) {
    return p;
}

void Transmitter::send(int16_t _queue) {
    // TODO: only send the state changed by this queuei
    _clients[_queue].send(inoperations);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Transmitter)
