#include "server.hh"
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>


Server::Server(uint16_t port) {
    _port = port;
}

void Server::_init_socket() {
    _socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (_socket_fd < 0)
        return _init_socket_error("socket");

    // Set TCP_NODELAY
    int yes = 1;
    setsockopt(_socket_fd, IPPROTO_TCP, TCP_NODELAY, (char *) &yes, sizeof(int));

    int sock_opt = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&sock_opt, sizeof(sock_opt)) < 0) {
        DEBUG("Set socket error...");
    }//if

    bzero((char*) &_sa, sizeof(_sa));
    _sa.sin_family = AF_INET;
    _sa.sin_port = htons(_port);
    _sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(_socket_fd, (struct sockaddr *)&_sa, sizeof(_sa)) < 0) {
        DEBUG("Error on binding socket...");
        return;
    }//if

    listen(_socket_fd, CON_QUEUE);
}

void Server::_interact(int conn_fd) {
//    auto n = recv(conn_fd, &_state, sizeof(State), 0);
//    if (n < 0) {
//        LOG("Read error\n");
//        return;
//    }//if
//
//    LOG("Here\n");
    auto n = read(conn_fd, &_state, sizeof(State));
    if (n < 0) {
        LOG("Read error\n");
        return;
    }//if

    char c = 'c';
    write(conn_fd, &c , sizeof(char));
}


void Server::_accept_connection() {
    socklen_t len = sizeof(_ca);
    int conn_fd = accept(_socket_fd, (struct sockaddr *) &(_ca), &len);

    if (conn_fd < 0) {
        printf("Error on accepting connection\n");
        return;
    }//if

    printf("A connection (%d) is accepted!\n", conn_fd);

    while (true) {
        _interact(conn_fd);
    }//while
}

void Server::_init_socket_error(const char *) {
    if (_socket_fd >= 0) {
        close(_socket_fd);
        _socket_fd = -1;
    }//if
}

void  Server::listen_to_state() {
    _init_socket();
    _accept_connection();
}