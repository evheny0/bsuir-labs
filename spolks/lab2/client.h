#ifndef CLIENT_H
#define CLIENT_H

#include "basic_socket_handler.h"

class Client: public BasicSocketHandler {
  public:
    Client(char *ip, int port);
    ~Client();
    
};

#endif // CLIENT_H
