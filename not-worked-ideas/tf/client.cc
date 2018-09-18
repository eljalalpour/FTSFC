#include <click/config.h>
#include <click/router.hh>
#include <clicknet/tcp.h>
#include <click/args.hh>
#include "client.hh"

CLICK_DECLS

Client::Client() { };

Client::~Client() {
    _close_socket();
};


Packet *Client::simple_action(Packet *p) {
    return p;
}

void Client::send(State& state) {
    write(_conn.socket, CAST_TO_BYTES(state), sizeof(State));

    // Wait for the response
    char c;
    read(_conn.socket, &c, sizeof(char));
//        DEBUG("Read from socket: %c", c);
}

int Client::_connect() {
    // Create socket
    DEBUG("Connecting to server %s on port %d", _conn.ip.c_str(), _conn.port);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        return 0;
    }//if

    // Set TCP_NODELAY
    int yes = 1;
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &yes, sizeof(int))) {
        perror("\nUnable to set TCP_NODELAY\n");
        return 0;
    }//if

    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));
    server.sin_addr.s_addr = inet_addr(_conn.ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(_conn.port);

    // inet_pton
    if(inet_pton(AF_INET, _conn.ip.c_str(), &server.sin_addr) <= 0) {
        perror("\n inet_pton error occured\n");
        return 0;
    }//if

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 0;
    }//if

    return sock;
}


void Client::_close_socket() {
    close(_conn.socket);
}


int Client::configure(Vector<String> &conf, ErrorHandler *) {

    BoundedIntArg b_int(0, 0xFFFF);
    string line(conf[0].c_str());
    vector<string> strs;
    Util _util;
    _util._split(line, ':', strs);
    _conn.ip = strs[0];
    b_int.parse(String(strs[1].c_str()), _conn.port);

    _conn.socket = _connect();

    return 0;
}

Packet *Transmitter::simple_action(Packet *p) {
    return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Client)