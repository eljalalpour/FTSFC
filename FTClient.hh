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
    FTTimestampState ft_timestamp;
};

class FTClient {
private:
    std::vector<pthread_t> _threads;
    std::vector<string> _ips;
    std::vector<uint16_t> _ports;

    static void* _send(void* param) {
        ServerConn* scp = static_cast<ServerConn*>(param);

        DEBUG("Connecting to server on port %d", scp->port);

        // Serialize state
        stringstream buffer;
        boost::archive::binary_oarchive oa(buffer);

        oa << scp->ft_timestamp;

        // Create socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            perror("Could not create socket");
            return NULL;
        }//if

        struct sockaddr_in server;
        server.sin_addr.s_addr = inet_addr(scp->ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(scp->port);

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

    bool send(FTTimestampState ft_timestamp) {
        bool result = true;
        pthread_attr_t attr;
        void *status;

        // Initialize and set thread joinable
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        for (size_t i = 0; i < _ips.size(); ++i) {
            ServerConn *conn = new ServerConn();
            conn->ft_timestamp.timestamp = ft_timestamp.timestamp;
            conn->ft_timestamp.state = ft_timestamp.state;
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