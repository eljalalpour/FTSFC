#include "FTStateElement.hh"
#include "FTAppenderElement.hh"
#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "monitorp.hh"
#include "Transmitter.hh"

CLICK_DECLS

Monitorp::Monitorp() : _counter(INIT_COUNTER) {};

Monitorp::~Monitorp() {};

Packet *Monitorp::simple_action(Packet *p) {
    LOG("--------------------");
//    std::cout << "Begin Monitorp :" << _id << std::endl;
    Router *r = this->router();

    //TODO: change the name of stateElement back to se
    Transmitter *stateElement = (Transmitter *)(r->find("trans"));

    //Getting the state's value from the FTStateElement
    stringstream ss;
    string value;
stringstream ss2;
    ss2 << (_counter + 1);
    ss2 >> value;

    FTState state;
    state[COUNTER] = value;
    _client.send(state);

    _counter++;

    LOG("Packet id is: %llu", FTAppenderElement::getPacketId(p));
    LOG("Packet counter read is %d, and written %d", _counter - 1, _counter);

    LOG("End Monitorp %d:", _id);
    LOG("--------------------");

    return p;
}

void Monitorp::_split(string& str, char dlm, vector<string>& tokens) {
    int last_index = 0;
    int i = 0;
    for (; i < str.size(); ++i) {
        if (str[i] == dlm && i - last_index > 0) {
            string token(str, last_index, i - last_index);
            tokens.push_back(token);
            last_index = i + 1;
        }//if
    }//for

    if (i - last_index > 0) {
        string token(str, last_index, i - last_index);
        tokens.push_back(token);
        last_index = i + 1;
    }//if
}

int Monitorp::configure(Vector<String> &conf, ErrorHandler *errh) {
//    if (Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX).complete() < 0)
//        return -1;

    Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX);

    LOG("Monitorp id is %d!\n", _id);

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

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Monitorp)
                                                        

