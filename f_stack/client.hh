#pragma once

#include <ff_api.h>

class Client {
private:
    int _socket;

public:
    void send(State&);
    bool connect(const string&, uint16_t);
};