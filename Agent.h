#pragma once
#include <string>

using std::string;

#define DEFAULT_SERVER_PORT 10000
#define DEFAULT_CLIENT_PORT_MIN 20000
#define DEFAULT_CLIENT_PORT_MAX 20100
#define CLICK_COMMAND_PREFIX "click "

// Messages to the click instance
#define ROLLBACK    0x01
#define STATE_QUERY 0x02

// Messages to the orchestrator
#define CLICK_INSTANCE_RUNNING 0x10

class Agent{
private:
    int _server_port;
    int _client_port;

    void _listenToServer();

public:
    Agent(int server_port = DEFAULT_SERVER_PORT);

};