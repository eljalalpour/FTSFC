#include "client.hh"

void Client::send(State& state) {
    // Send state
    write(_socket, reinterpret_cast<unsigned char *>(_state_to_be_sent), sizeof(State));

    char c;
    read(_socket, &c, sizeof(char));
}

bool Client::connect(const string& ip, uint16_t port) {
    bool result = true;

    // Create socket
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        perror("Could not create socket");
        result = false;
        goto CLEANUP;
    }//if

    // Set TCP_NODELAY
    int yes = 1;
    if (setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (char *) &yes, sizeof(int))) {
        perror("\nUnable to set TCP_NODELAY\n");
        result = false;
        goto CLEANUP;
    }//if

    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // inet_pton
    if(inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0) {
        perror("\n inet_pton error occured\n");
        result = false;
        goto CLEANUP;
    }//if

    // Connect to server
    if (connect(_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        result = false;
        goto CLEANUP;
    }//if
CLEANUP:
    return result;
}