#pragma once

#include <vector>
#include <string>
#include <pthread.h>
#include <arpa/inet.h>
#include "FTTypes.hh"
#include "FTAppenderElement.hh"

struct ServerConn {
    string ip;
    int port;
    FTState state;
};

class FTClient {
private:
    std::vector<pthread_t> _threads;
    std::vector<string> _ips;
    std::vector<uint16_t> _ports;

    static void* _send(void* param) {
        ServerConn* scp = static_cast<ServerConn*>(param);

        // Serialize state
        stringstream buffer;
        boost::archive::binary_oarchive oa(buffer);

        oa << scp->state;

        // Create socket
        DEBUG("Connecting to server %s on port %d", scp->ip.c_str(), scp->port);
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            perror("Could not create socket");
            return NULL;
        }//if

        struct sockaddr_in server;
        memset(&server, '0', sizeof(server));
        server.sin_addr.s_addr = inet_addr(scp->ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(scp->port);

        // inet_pton
        if(inet_pton(AF_INET, scp->ip.c_str(), &server.sin_addr) <= 0) {
            perror("\n inet_pton error occured\n");
            return NULL;
        }//if

        // Connect to server
        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
            perror("connect failed. Error");
            return NULL;
        }//if

        // Send state
        size_t size = buffer.str().size();
        write(sock, &size, sizeof(size_t));
        write(sock, buffer.str().c_str(), size);

        // Wait for the response
        char c;
        read(sock, &c, sizeof(char));
        close(sock);
        DEBUG("Read from socket: %c", c);

        return NULL;
    }

public:
    FTClient() { };

    FTClient(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        set_ip_ports(ips, ports);
    }

    bool send(FTState state) {
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

    void set_ip_ports(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        _ips.clear();
        _ports.clear();

        _ips.insert(_ips.begin(), ips.begin(), ips.end());
        _ports.insert(_ports.begin(), ports.begin(), ports.end());
    }
};