#ifndef BASIC_SOCKET_HANDLER_H
#define BASIC_SOCKET_HANDLER_H

#include "socket.h"

class BasicSocketHandler {

  protected:
    Socket *_socket_ptr;
    struct sockaddr_in _server_addres;
  public:
    BasicSocketHandler();
    ~BasicSocketHandler();
};

#endif // BASIC_SOCKET_HANDLER_H
