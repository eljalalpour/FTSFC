#pragma once

#include <vector>
#include <string>
#include <pthread.h>
#include "FTTypes.hh"

struct ServerConn {
    string ip;
    uint16_t port;
    FTTimestampState ft_timestamp;
};

class FTClient {
private:
    std::vector<pthread_t> _threads;
    std::vector<string> _ips;
    std::vector<uint16_t> _ports;

    static void* _send(void* param);

public:
    FTClient(std::vector<string>& ips, std::vector<uint16_t>& ports);
    bool send(FTTimestampState& ft_timestamp);
};