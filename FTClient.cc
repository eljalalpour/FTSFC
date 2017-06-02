#include "FTClient.hh"
#include "FTAppenderElement.hh"
#include <arpa/inet.h>

FTClient::FTClient(std::vector<string>& ips, std::vector<int>& ports) : _ips(ips), _ports(ports) { }

void* FTClient::_send(void* param) {
    ServerConn* scp = static_cast<ServerConn*>(param);

    // Serialize state
    stringstream buffer;
    boost::archive::binary_oarchive oa(buffer);
    oa << scp->ft_timestamp.state;

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

    return NULL;
}

bool FTClient::send(FTTimestampState& ft_timestamp) {
    pthread_attr_t attr;
    void *status;

    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (size_t i = 0; i < _ips.size(); ++i) {
        ServerConn conn;
        conn.ft_timestamp = ft_timestamp;
        conn.ip = _ips[i];
        conn.port = _ports[i];

        pthread_t thread;
        if(!pthread_create(&thread, &attr, _send, (void *)&conn)) {
            goto CLEANUP;
        }//if
        _threads.push_back(thread);
    }

    // free attribute and wait for the other threads
    pthread_attr_destroy(&attr);

    for(size_t i = 0; i < _ips.size(); ++i) {
        if (!pthread_join(_threads[i], &status))
            goto CLEANUP;
    }//for

CLEANUP:
    pthread_exit(NULL);
}
