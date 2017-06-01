#pragma once

#include <vector>
#include <string>
#include <pthread.h>
#include "FTTypes.hh"

struct ServerConn {
    string ip;
    int port;
    FTTimestampState ft_timestamp;
};

class FTClient {
private:
    std::vector<pthread_t> _threads;
    std::vector<string> _ips;
    std::vector<int> _ports;

    static void* _send(void* param);

public:
    FTClient(std::vector<string>& ips, std::vector<int>& ports);
    bool send(FTTimestampState& ft_timestamp);
};