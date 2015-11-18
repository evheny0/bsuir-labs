#ifndef CLIENT_CONNECTION_STATE_H
#define CLIENT_CONNECTION_STATE_H

#include <iostream>
#include <fstream>
#include "socket.h"
#include <chrono>

enum CONNECTION_TYPE {
    TCP_CONNECTION,
    UDP_CONNECTION
};


class ClientConnectionState {
    Socket *_socket_ptr;
    std::ifstream _file;
    long _last_read;
    struct sockaddr _remote_addr;
    CONNECTION_TYPE _connection_type;
    bool _closed;

  public:
    ClientConnectionState(Socket *socket);
    ClientConnectionState();
    ~ClientConnectionState();
    void open_file(const char *str);
    std::ifstream &get_file();
    Socket *get_socket();
    SOCKET get_socket_obj();
    void set_file_position(std::ifstream::pos_type &position);
    void revert_last_file_read();
    bool is_eof();
    bool is_closed();
    void set_closed();
    void read_file(char *buffer, long size);
    long file_gcount();
    long last_position();
    struct sockaddr *get_sockaddr();
    bool is_tcp();
    bool is_udp();
};

#endif // CLIENT_CONNECTION_STATE_H
