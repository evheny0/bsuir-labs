#ifndef CLIENT_H
#define CLIENT_H

#include "basic_socket_handler.h"

class Client: public BasicSocketHandler {
  public:
    Client();
    ~Client();
    
};

#endif // CLIENT_H
