#pragma once

#include <vector>
#include <string>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "defs.hh"

using std::string;

typedef struct {
    string ip;
    uint16_t port;
    int socket;
} ServerConn;

using std::vector;
using std::string;

CLICK_DECLS

class Client : public Element {
private:
    ServerConn _conn;

    int _connect(string ip, uint16_t port);

    void _close_socket();

public:
    Client();

    ~Client();

    const char *class_name() const { return "Client"; }

    const char *port_count() const { return PORTS_0_0; }

    const char *processing() const { return AGNOSTIC; }

    Packet *simple_action(Packet *p);

    void send(State&);

    int configure(Vector<String> &, ErrorHandler *);
};

CLICK_ENDDECLS
