#pragma once

#include <click/config.h>
#include <click/element.hh>
#include <click/router.hh>
#include <arpa/inet.h>
#include <vector>
#include "FTTypes.hh"
#include "FTStateElement.hh"
#include "FTAppenderElement.hh"

CLICK_DECLS

#define CON_QUEUE  1
#define SE_ELEMENT "se"
#define GET_STATE_CMD 0x02
#define PUT_STATE_CMD 0x03

class FTControlElement: public Element {
private:
    uint16_t _port;
    int _socket_fd;
    struct sockaddr_in _sa;
    struct sockaddr_in _ca;
    pthread_t _accept_thread;

    void _init_socket();
    void _init_socket_error(const char *syscall);
    static void* _accept_connection(void* param);

public:
    FTControlElement();
    ~FTControlElement();

    const char *class_name() const { return "FTControlElement"; }
    const char *port_count() const { return PORTS_0_0; }
    const char *processing() const { return AGNOSTIC; }
    int configure(Vector<String> &, ErrorHandler *);

    void put(FTStateElement* se, int conn_fd);
    void get(FTStateElement* se, int conn_fd);
};

CLICK_ENDDECLS