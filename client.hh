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
    State* state;
} ServerConn;

class Client {
private:
    std::vector<pthread_t> _threads;
//    std::vector<string> _ips;
//    std::vector<uint16_t> _ports;
//    std::vector<int> _sockets;
    std::vector<ServerConn> _conns;

    static void* _send(void* param) {
        ServerConn* scp = static_cast<ServerConn*>(param);

        // Send state
        write(scp->socket, CAST_TO_BYTES(scp->state), sizeof(State));

//        // Wait for the response
        char c;
        read(scp->socket, &c, sizeof(char));
//        DEBUG("Read from socket: %c", c);

//        send(scp->socket, CAST_TO_BYTES((scp->state), sizeof(State), 0);

        return 0;
    }

    int _connect(string ip, uint16_t port) {
        // Create socket
        DEBUG("Connecting to server %s on port %d", ip.c_str(), port);
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
        server.sin_addr.s_addr = inet_addr(ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        // inet_pton
        if(inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0) {
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

    void _connect_sockets() {
        for (size_t i = 0; i < _conns.size(); ++i) {
            _conns[i].socket = _connect(_conns[i].ip, _conns[i].port);
        }//for
    }

    void _close_sockets() {
        for (size_t i = 0; i < _conns.size(); ++i)
            close(_conns[i].socket);
    }

public:
    Client() { };

    ~Client() {
        _close_sockets();
    }

    Client(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        set_ip_ports(ips, ports);
    }

    bool multi_send(State &state) {
        bool result = true;
        pthread_attr_t attr;
        void *status;

        // Initialize and set thread joinable
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        for (size_t i = 0; i < _conns.size(); ++i) {
            pthread_t thread;
            _conns[i].state = &state;
            if(pthread_create(&thread, &attr, _send, (void *)&_conns[i])) {
                DEBUG("Error on creating a thread for the server on %s:%d", _conns[i].ip.c_str(), _conns[i].port);
                result = false;
                goto CLEANUP;
            }//if
            _threads.push_back(thread);
        }//for

        // free attribute and wait for the other threads
        pthread_attr_destroy(&attr);

        for(size_t i = 0; i < _conns.size(); ++i) {
            if (pthread_join(_threads[i], &status)) {
                DEBUG("Error on joining the thread on %s:%d", _conns[i].ip.c_str(), _conns[i].port);
                result = false;
                goto CLEANUP;
            }//if
        }//for

    CLEANUP:
        _threads.clear();
        return result;
    }

    bool send(State& state) {
        if (_conns.size() > 1) {
            return multi_send(state);
        }//if

        _conns[0].state = &state;

        _send(&_conns[0]);
        return true;
    }

    void set_ip_ports(std::vector<string>& ips, std::vector<uint16_t>& ports) {
//        _ips.clear();
//        _ports.clear();
//
//        _ips.insert(_ips.begin(), ips.begin(), ips.end());
//        _ports.insert(_ports.begin(), ports.begin(), ports.end());

        for (size_t i = 0; i < ips.size(); ++i) {
            ServerConn conn;
            conn.ip = ips[i];
            conn.port = ports[i];
            _conns.push_back(conn);
        }//for

        _connect_sockets();
    }
};