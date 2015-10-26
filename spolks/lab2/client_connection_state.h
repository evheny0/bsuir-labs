#ifndef CLIENT_CONNECTION_STATE_H
#define CLIENT_CONNECTION_STATE_H

#include <iostream>
#include <fstream>
#include "socket.h"


class ClientConnectionState {
    Socket *_socket_ptr;
    std::ifstream _file;

  public:
    ClientConnectionState(Socket *socket);
    ~ClientConnectionState();
    void open_file(const char *str);
    std::ifstream &get_file();
    void set_file_position(std::ifstream::pos_type &position);

};

#endif // CLIENT_CONNECTION_STATE_H
