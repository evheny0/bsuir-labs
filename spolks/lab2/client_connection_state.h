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

  public:
    ClientConnectionState(Socket *socket);
    ~ClientConnectionState();
    void open_file(const char *str);
    std::ifstream &get_file();
    Socket *get_socket();
    void set_file_position(std::ifstream::pos_type &position);
    void set_deleted(bool value);
    bool is_deleted();
};

#endif // CLIENT_CONNECTION_STATE_H
