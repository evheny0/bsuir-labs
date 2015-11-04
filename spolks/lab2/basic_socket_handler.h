#ifndef BASIC_SOCKET_HANDLER_H
#define BASIC_SOCKET_HANDLER_H

#include "socket.h"
#include "utils.h"
#include <iostream>
#include <fstream>

const char CHECK_CODE[5] = "\n";
const int CHECK_CODE_SIZE = 1;

class BasicSocketHandler {

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
    void recieve_raw_package_from(Socket *from_socket, Package &package, int size = BUFFER_MESSAGE_SIZE);
    int send_package_to(Socket *to_socket, Package &package);
    int send_raw_package_to(Socket *to_socket, Package &package);

};

#endif // BASIC_SOCKET_HANDLER_H
