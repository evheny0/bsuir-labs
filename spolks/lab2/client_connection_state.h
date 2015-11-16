#ifndef CLIENT_CONNECTION_STATE_H
#define CLIENT_CONNECTION_STATE_H

#include <iostream>
#include <fstream>
#include "socket.h"
#include <chrono>


class ClientConnectionState {
    Socket *_socket_ptr;
    std::ifstream _file;
    bool _deleted;
    long _last_read;

  public:
    ClientConnectionState(Socket *socket);
    ~ClientConnectionState();
    void open_file(const char *str);
    std::ifstream &get_file();
    Socket *get_socket();
    SOCKET get_socket_obj();
    void set_file_position(std::ifstream::pos_type &position);
    void revert_last_file_read();
    void set_deleted(bool value);
    bool is_deleted();
    bool is_eof();
    void read_file(char *buffer, long size);
    long file_gcount();
};

#endif // CLIENT_CONNECTION_STATE_H
