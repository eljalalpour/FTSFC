#pragma once

#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define CON_QUEUE    10
#define DEFAULT_PORT 10000

using std::vector;

class Server {
private:
    uint16_t _port;
    int _socket_fd;
    struct sockaddr_in _sa;
    struct sockaddr_in _ca;
    State _state;

    void _init_socket();
    void _init_socket_error(const char *syscall);
    void _accept_connection();
    void _interact(int conn_fd);

public:
    Replica(uint16_t port);
    void listen_to_state();
};
