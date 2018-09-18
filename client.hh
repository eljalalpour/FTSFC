#pragma once

#include <vector>
#include <string>
#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <thread>
#include "defs.hh"

using std::string;
using std::thread;

// Note that n and k must be of type size_t
#define K_TH_BIT(n,k)       ((n & ( 1 << k )) >> k)
#define RESET_K_TH_BIT(n,k) (n &= ~(1UL << k))
#define SET_K_TH_BIT(n,k)   (n |= 1UL << k)
#define SET_ALL_BITS -1

typedef struct {
    string ip;
    uint16_t port;
    int socket;
    State* state;
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

    State* _state_to_be_sent;

//    static void* _send(void* param) {
//        ServerConn* scp = static_cast<ServerConn*>(param);
//
//        // Send state
//        write(scp->socket, CAST_TO_BYTES(scp->state), sizeof(State));
//
//        // Wait for the response
//        char c;
//        read(scp->socket, &c, sizeof(char));
//        return 0;
//    }

    void _send(ServerConn& scp, State& _state_to_be_sent) {
        // Send state
        LOG("Sending...");
        write(scp.socket, CAST_TO_BYTES(_state_to_be_sent), sizeof(State));

        LOG("Receiving...");
        char c;
        read(scp.socket, &c, sizeof(char));
    }

    void _send_for_ever(int _id) {
        while(true) {
            {// Wait until there is some state to be sent
                std::unique_lock<std::mutex> lock(_ready_mtx);
                // The worker also checks its
                _ready_cv.wait(lock, [&](){ return K_TH_BIT(_ready, _id) > 0;});
            }//{

            // Send state to be replicated
            _send(_conns[_id], *_state_to_be_sent);

            LOG("To wake up client!");
            {// Let the Client know that this process has finished its task
                std::lock_guard<std::mutex> lock_guard(_pending_workers_mtx);
                -- _pending_workers;
                LOG("Pending workers: %d", _pending_workers);
                if (_pending_workers == 0) {//if no other sender exists, notify the client
                    {
                        std::lock_guard<std::mutex> lock(_ready_mtx);
                        // Clear _ready bitset
                        _ready = 0;
                    }//{
                    _pending_workers_cv.notify_one();
                    LOG("Notify the client");
                }//if
                else {
                    {// Remember that it already processed the data
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

        LOG("Connected to %s:%d", ip.c_str(), port);

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
            _threads.push_back(std::thread(&Client::_send_for_ever, this, i));
        }//for
    }

public:
    Client() {
        _ready = false;
        _pending_workers = 0;

    };

    ~Client() {
        _close_sockets();
    }

    Client(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        set_ip_ports(ips, ports);
    }

//    bool multi_send(State &state) {
//        bool result = true;
//        pthread_attr_t attr;
//        void *status;
//
//        // Initialize and set thread joinable
//        pthread_attr_init(&attr);
//        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
//
//        for (size_t i = 0; i < _conns.size(); ++i) {
//            pthread_t thread;
//            _conns[i].state = &state;
//            if(pthread_create(&thread, &attr, _send, (void *)&_conns[i])) {
//                DEBUG("Error on creating a thread for the server on %s:%d", _conns[i].ip.c_str(), _conns[i].port);
//                result = false;
//                goto CLEANUP;
//            }//if
//            _threads.push_back(thread);
//        }//for
//
//        // free attribute and wait for the other threads
//        pthread_attr_destroy(&attr);
//
//        for(size_t i = 0; i < _conns.size(); ++i) {
//            if (pthread_join(_threads[i], &status)) {
//                DEBUG("Error on joining the thread on %s:%d", _conns[i].ip.c_str(), _conns[i].port);
//                result = false;
//                goto CLEANUP;
//            }//if
//        }//for
//
//    CLEANUP:
//        _threads.clear();
//        return result;
//    }

    void multi_send(State& state) {
        LOG("Multi send");
        LOG("---------------------");
        {// Produce the state and notify the senders
            _state_to_be_sent = &state;
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

    void send(State& state) {
        if (_conns.size() > 1) {
            return multi_send(state);
        }//if
        else {
            _send(_conns[0], state);
        }//else
    }

    // This function must be called only once
    void set_ip_ports(std::vector<string>& ips, std::vector<uint16_t>& ports) {
        for (size_t i = 0; i < ips.size(); ++i) {
            ServerConn conn;
            conn.ip = ips[i];
            conn.port = ports[i];
            _conns.push_back(conn);
        }//for

        _pending_workers = _conns.size();
        _ready = false;

        _connect_sockets();
        _create_threads();
    }
};