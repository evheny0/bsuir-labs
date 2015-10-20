#ifndef BASIC_SOCKET_HANDLER_H
#define BASIC_SOCKET_HANDLER_H

#include "socket.h"

class BasicSocketHandler {

  protected:
    Socket *_socket_ptr;
    struct sockaddr_in _server_addres;
    char *ip;
    int port;
  public:
    BasicSocketHandler(char *ip, int port);
    ~BasicSocketHandler();
    struct sockaddr_in build_server_address(char *ip, int port);

};

#endif // BASIC_SOCKET_HANDLER_H
