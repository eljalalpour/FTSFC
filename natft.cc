#include "FTStateElement.hh"
#include "FTAppenderElement.hh"
#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "natft.hh"
#include "Transmitter.hh"

#include <clicknet/ip.h>
#include <clicknet/tcp.h>
#include <clicknet/udp.h>
#include <click/straccum.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <click/ipflowid.hh>
#include <boost/range/irange.hpp>
#include <map>

CLICK_DECLS

natft::natft(){};

natft::~natft() {};

//Packet *natft::simple_action(Packet *p) {
void 
natft::push(int port, Packet *p) {
//    LOG("--------------------");
    Router *r = this->router();

    //TODO: change the name of Transmitter to "trans"
    Transmitter *stateElement = (Transmitter *)(r->find("trans"));

    //Getting the state's value from the FTStateElement
    stringstream ss, ss1;
    string value;

    //Extract the five column and construct key
    WritablePacket *rp = p->uniqueify();
    click_ip *iph = rp->ip_header();

    printf("before!!\n");
    
    //Eaxmine packet header and del
    // if not tcp or udp, delete it
    if((iph->ip_p != IP_PROTO_TCP && iph->ip_p != IP_PROTO_UDP)
	|| !IP_FIRSTFRAG(iph)
	|| rp->transport_length() < 8) {
    	printf("delete one \n");
	rp->kill();
    	printf("delete one \n");
	return;
    }    

    // get the flow id
    IPFlowID flowid(rp);

    // parse the five column
    ss << flowid.saddr().addr() << " ";
    ss << flowid.sport() << " ";
    ss << flowid.daddr().addr() << " ";
    ss << flowid.dport() << " ";
    // ss << "protocol" << "/";

    string teststr = ss.str(); 
//    std::cout << "key is" << teststr << std::endl;
    //Search the key from database
    std::map<string, string>::iterator it;
    it = _table.find(teststr);
    if (it != _table.end()) {
//    	printf("found! \n");
        if (it->second.size() != 0) {
//	    std::cout << "found" << std::endl;
            
	    ss1 << it->second;
	    // get the destinatnftion ip addr
	    uint32_t addrs[4];

	    for (auto i : boost::irange(0, 4)) {
	    	ss1 >> addrs[i];
	    }
            // rewrite the package header
	    p->set_anno_u32(0, addrs[2]);
    	
	    printf("FOUND!!!!!\n");

        } 
    } else {//if 
	// generate new flowID
        // hardcode dst ip to 192.168.1.101
        uint32_t dst_ip = 3232235877;

        ss1 << flowid.saddr().addr() << " ";
        ss1 << flowid.sport() << " ";
        ss1 << dst_ip << " ";
        ss1 << flowid.dport() << " ";

        printf("NOT FOUND\n");
            // put new flowID into database
	FTState state;
	state[ss.str()] = ss1.str();
	_client.send(state);
        //stateElement->send(ss.str(), ss1.str());
	_table[ss.str()] = ss1.str();
	
            /// if tcp put reply flow into database

            // rewrite the package header
        p->set_anno_u32(0, dst_ip);

    }
    //stateElement->putPrimaryState(ss.str(), value);

    LOG("--------------------");

//    return p;
//    printf("push to port\n");
    output(port).push(p);
}

void natft::_split(string& str, char dlm, vector<string>& tokens) {
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

int 
natft::configure(Vector<String> &conf, ErrorHandler *errh) {
//    if (Args(conf, this, errh).read_or_set("ID", _id, rand() % MB_ID_MAX).complete() < 0)
//        return -1;

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
//    _print_ip_port_list();

    LOG("natft id is %d!\n", _id);

    return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(natft)
