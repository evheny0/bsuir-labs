#ifndef BASIC_SOCKET_HANDLER_H
#define BASIC_SOCKET_HANDLER_H

#include "socket.h"
#include "utils.h"
#include <iostream>
#include <fstream>

const char CHECK_CODE[5] = "\n";
const int CHECK_CODE_SIZE = 1;
const char FILENAME_RECIEVE[100] = "file_to_recieve.jpg";
const int CYCLES_FOR_PRINT = 100;


class BasicSocketHandler {

  protected:
    struct sockaddr_in _server_addres;
    char *ip;
    int port;
    Package _package;

  public:
    BasicSocketHandler(const char *ip, int port);
    ~BasicSocketHandler();
    struct sockaddr_in build_server_address(const char *ip, int port);
    void set_recieving_timeout(Socket *socket);
    Package recieve_package_from(Socket *from_socket);
    void recieve_raw_package_from(Socket *from_socket, Package &package, int size = BUFFER_MESSAGE_SIZE, struct sockaddr *src_addr = NULL);
    int send_package_to(Socket *to_socket, Package &package);
    int send_raw_package_to(Socket *to_socket, Package &package, struct sockaddr *src_addr = NULL);

};

#endif // BASIC_SOCKET_HANDLER_H
