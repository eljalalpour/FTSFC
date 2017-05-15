#include <click/config.h>
#include "FTControlElement.hh"
#include <click/args.hh>
CLICK_DECLS

FTControlElement::FTControlElement() { }

FTControlElement::~FTControlElement() { }

int FTControlElement::configure(Vector<String> &conf, ErrorHandler * errh) {
    BoundedIntArg parser(0, 0xFFFF);
    parser.parse(conf[0], _port);

    _init_socket();

    return 0;
}

void FTControlElement::_init_socket() {
    _socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (_socket_fd < 0)
        return _init_socket_error("socket");
    int sockopt = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&sockopt, sizeof(sockopt)) < 0) {
        click_chatter("Set socket error: %s", strerror(errno));
    }//if

    bzero((char*) &_sa, sizeof(_sa));
    _sa.sin_family = AF_INET;
    _sa.sin_port = htons(_port);
    _sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(_socket_fd, (struct sockaddr *)&_sa, sizeof(_sa)) < 0)
        click_chatter("Error on binding");

    listen(_socket_fd, CON_QUEUE);
    pthread_create(&_accept_thread, NULL, _accept_connection, this);
}

void FTControlElement::put(FTStateElement* se, int conn_fd) {
    // First read the id of the middlebox,
    // then the size of the state,
    // and finally the state!
    FTMBId id;
    read(conn_fd, &id, sizeof(FTMBId));

    int size = 0;
    read(conn_fd, &size, sizeof(int));
    size = ntohl(size);

    char buffer[size];
    read(conn_fd, &buffer, size);

    click_chatter("State is (%d): ", size);
    for(int i = 0; i < size; i++)
        click_chatter("%d ", buffer[i]);

    if (size > 0) {
        FTState state;
        FTAppenderElement::decode(buffer, size, state);

        FTAppenderElement::printState(state);

        se->putCommittedState(id, state);
    }//if
}

void FTControlElement::get(FTStateElement* se, int conn_fd) {
    FTMBId id;
    read(conn_fd, &id, sizeof(FTMBId));
    FTState state;
    if (se->getCommittedState(id, state)) {

        string buffer;
        FTAppenderElement::encode(state, buffer);
        int size = buffer.size();
        write(conn_fd, &size, sizeof(int));
        write(conn_fd, buffer.c_str(), size);

        click_chatter("State is (%d): ", buffer.size());
        FTAppenderElement::printState(state);
        for(int i = 0; i < buffer.size(); i++)
            click_chatter("%d ", buffer[i]);
    }//if
    else {
        // If there is some error in finding the state, return 0
        int size = 0;
        write(conn_fd, &size, sizeof(int));
    }//else
}

void* FTControlElement::_accept_connection(void* param) {
    FTControlElement* tp = static_cast<FTControlElement*>(param);
    while(true) {
        socklen_t len = sizeof(tp->_ca);
        int conn_fd = accept(tp->_socket_fd, (struct sockaddr *) &(tp->_ca), &len);

        click_chatter("A connection (%d) is accepted!", conn_fd);

        if (conn_fd < 0) {
            click_chatter("Error on accepting connection");
            break;
        }//if

        FTStateElement* se = static_cast<FTStateElement*>(tp->router()->find(SE_ELEMENT));
        uint8_t command;
        read(conn_fd, &command, sizeof(uint8_t));

        click_chatter("Command is %d", command);

        switch(command) {
            case GET_STATE_CMD:
                click_chatter("Get state command!", conn_fd);
                tp->get(se, conn_fd);
                break;

            case PUT_STATE_CMD:
                click_chatter("Put state command!", conn_fd);
                tp->put(se, conn_fd);
                break;

            default:
                click_chatter("Unknown command");
                break;
        }//switch

    }//while
}

void FTControlElement::_init_socket_error(const char *syscall) {
    int e = errno;

    if (_socket_fd >= 0) {
        close(_socket_fd);
        _socket_fd = -1;
    }//if

    click_chatter("%s: %s", syscall, strerror(e));
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FTControlElement)