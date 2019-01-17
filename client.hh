#pragma once

#include <vector>
#include <string>
#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h>
//#include <sys/socket.h>
//#include <sys/types.h>
#include "defs.hh"

#define TF_CHUNK_SIZE 74
typedef State TFStates[TF_CHUNK_SIZE];

using std::string;
using std::thread;

typedef struct ServerConn {
    string ip;
    uint16_t port;
    int socket;
    
    ServerConn(string ip = "", uint16_t port = 0, int socket = 0) {
        this->ip = ip;
        this->port = port;
        this->socket = socket;
    }
} ServerConn;

class Client {
private:
    std::vector<std::thread> _threads;
    std::vector<ServerConn> _conns;

    /// \texttt{_ready} shows that if the data is ready for consumption
    /// Further, \texttt{_ready} is a bitset whose i-th bit shows that whether
    /// i-th thread has already consumed the data
    volatile size_t _ready;
    std::condition_variable _ready_cv;
    std::mutex _ready_mtx;

    volatile size_t _pending_workers;
    std::condition_variable _pending_workers_cv;
    std::mutex _pending_workers_mtx;

//    State* _state_to_be_sent;
    TFStates* _states;

    void _send(ServerConn& scp, TFStates& _state_to_be_sent) {
	while(true) {
	        // Send state
        	auto status = write(scp.socket, CAST_TO_BYTES(_state_to_be_sent), sizeof(TFStates));
	        if (status == -1) {
			LOG("Error occured during send!");
  		}//if
	        char c;
        	read(scp.socket, &c, sizeof(char));
	}
    }

    void _send_for_ever(int _id) {
        while(true) {
            {// Wait until there is some state to be sent
                std::unique_lock<std::mutex> lock(_ready_mtx);
                // The worker also checks its own bit
                _ready_cv.wait(lock, [&](){ return K_TH_BIT(_ready, _id) > 0;});
            }//{

            // Send state to be replicated
            _send(_conns[_id], *_states);

            {// Let the Client know that this process has finished its task
                std::lock_guard<std::mutex> lock_guard(_pending_workers_mtx);
                -- _pending_workers;
                if (_pending_workers == 0) {//if no other sender exists, notify the client
                    {
                        std::lock_guard<std::mutex> lock(_ready_mtx);
                        // Clear _ready bitset
                        _ready = 0;
                    }//{
                    _pending_workers_cv.notify_one();
                }//if
                else {
                    {// Remember that it have already processed the data
                        std::lock_guard<std::mutex> lock(_ready_mtx);
                        _ready = RESET_K_TH_BIT(_ready, _id);
                    }//{
                }//else
            }//}
        }//while
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
	
	int flags = fcntl(sock, F_GETFL, 0);
	if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
	    perror("set to non-blocking socket failed!");
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

    void _create_threads() {
        for (size_t i = 0; i < _conns.size(); ++i) {
            _threads.emplace_back(&Client::_send_for_ever, this, i);
        }//for
    }

    void _multi_send(TFStates &states) {
        {// Produce the state and notify the senders
            _states = &states;
            _pending_workers = _conns.size();

            std::unique_lock<std::mutex> lock(_ready_mtx);
            // Set all bits to 1, to let all the workers know that the data is ready for them
            _ready = SET_ALL_BITS;
            _ready_cv.notify_all();
        }// {
        {// Wait for senders to finish their tasks
            std::unique_lock<std::mutex> lock(_pending_workers_mtx);
            _pending_workers_cv.wait(lock, [&](){ return _pending_workers == 0;});
        }// {
    }

public:
    Client() {
        _ready = 0;
        _pending_workers = 0;
	_conns.reserve(MAX_QUEUES);
    }

    ~Client() {
        _close_sockets();
    }

    Client(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        set_ip_ports(ips, ports);
    }

    void send(TFStates& states) {
        if (_conns.size() > 1) {
            _multi_send(states);
        }//if
        else {
            _send(_conns[0], states);
        }//else
    }

    // This function must be called only once
    void set_ip_ports(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        for (size_t i = 0; i < ips.size(); ++i) {
            ServerConn conn;
            conn.ip = ips[i];
            conn.port = ports[i];
            _conns.push_back(conn);
//            _conns.emplace_back(ips[i], ports[i]);
        }//for
        _pending_workers = _conns.size();
        _ready = 0;
        _connect_sockets();
//        _create_threads();
    }
};
