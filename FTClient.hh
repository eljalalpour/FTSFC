#pragma once

#include <vector>
#include <string>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "FTTypes.hh"
#include "FTAppenderElement.hh"

struct ServerConn {
    string ip;
    uint16_t port;
    FTState state;
    int socket;
};

class FTClient {
private:
    std::vector<pthread_t> _threads;
    std::vector<string> _ips;
    std::vector<uint16_t> _ports;
    std::vector<int> _sockets;

    static void* _send(void* param) {
        ServerConn* scp = static_cast<ServerConn*>(param);

        // Serialize state
        string buffer;
        Serializer serializer;
        buffer = serializer.serialize(scp->state);

        // Send state
        size_t size = buffer.size();
        write(scp->socket, &size, sizeof(size_t));
        write(scp->socket, buffer.c_str(), size);

        // Wait for the response
        char c;
        read(scp->socket, &c, sizeof(char));
        DEBUG("Read from socket: %c", c);

        return NULL;
    }

    int _connect(string ip, uint16_t port) {
        // Create socket
        DEBUG("Connecting to server %s on port %d", ip.c_str(), port);
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            perror("Could not create socket");
            return NULL;
        }//if

        // Set TCP_NODELAY
        int yes = 1;
        if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &yes, sizeof(int))) {
            perror("\nUnable to set TCP_NODELAY\n");
            return NULL;
        }//if

        struct sockaddr_in server;
        memset(&server, '0', sizeof(server));
        server.sin_addr.s_addr = inet_addr(ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        // inet_pton
        if(inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0) {
            perror("\n inet_pton error occured\n");
            return NULL;
        }//if

        // Connect to server
        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
            perror("connect failed. Error");
            return NULL;
        }//if

        return sock;
    }

    void _connect_sockets() {
        for (int i = 0;i < _ips.size(); ++i)
            _sockets.push_back(_connect(_ips[i], _ports[i]));
    }

    void _close_sockets() {
        for (int i = 0; i < _sockets.size(); ++i)
            close(_sockets[i]);
    }

public:
    FTClient() { };

    ~FTClient() {
        _close_sockets();
    }

    FTClient(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        set_ip_ports(ips, ports);
    }

    bool multi_send(FTState &state) {
        bool result = true;
        pthread_attr_t attr;
        void *status;

        // Initialize and set thread joinable
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        for (size_t i = 0; i < _ips.size(); ++i) {
            ServerConn *conn = new ServerConn();
            conn->state = state;
            conn->ip = _ips[i];
            conn->port = _ports[i];
            conn->socket = _sockets[i];

            pthread_t thread;
            if(pthread_create(&thread, &attr, _send, (void *)conn)) {
                DEBUG("Error on creating a thread for the server on %s:%d", conn->ip.c_str(), conn->port);
                result = false;
                goto CLEANUP;
            }//if
            _threads.push_back(thread);
        }//for

        // free attribute and wait for the other threads
        pthread_attr_destroy(&attr);

        for(size_t i = 0; i < _ips.size(); ++i) {
            if (pthread_join(_threads[i], &status)) {
                DEBUG("Error on joining the thread on %s:%d", _ips[i].c_str(), _ports[i]);
                result = false;
                goto CLEANUP;
            }//if
        }//for

        CLEANUP:
        _threads.clear();
        return result;
    }

    bool send(FTState& state) {
        if (_ips.size() > 1) {
            return multi_send(state);
        }//if

        _send(status);
        return true;
    }

    void set_ip_ports(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        _ips.clear();
        _ports.clear();

        _ips.insert(_ips.begin(), ips.begin(), ips.end());
        _ports.insert(_ports.begin(), ports.begin(), ports.end());

        _connect_sockets();
    }
};