#ifndef BASIC_SOCKET_HANDLER_H
#define BASIC_SOCKET_HANDLER_H

#include "socket.h"
#include "utils.h"
#include <iostream>
#include <fstream>

const char CHECK_CODE[5] = "\n\n";
const int CHECK_CODE_SIZE = 2;

class BasicSocketHandler {
    std::string _last_data;
    int _last_data_size;
    Package _last_package;

  protected:
    Socket *_socket_ptr;
    struct sockaddr_in _server_addres;
    char *ip;
    int port;
  public:
    BasicSocketHandler(const char *ip, int port);
    ~BasicSocketHandler();
    struct sockaddr_in build_server_address(const char *ip, int port);
    Package recieve_package_from(Socket *from_socket);
    Package recieve_raw_package_from(Socket *from_socket);
    int send_package_to(Socket *to_socket, Package package);
    int send_raw_package_to(Socket *to_socket, Package package);

};

#endif // BASIC_SOCKET_HANDLER_H
