#ifndef SERVER_H
#define SERVER_H

#include "basic_socket_handler.h"

class Server: public BasicSocketHandler {
  public:
    Server();
    ~Server();
    
  private:
    struct sockaddr_in build_server_address();
    void assign_address_to_socket();
};

#endif // SERVER_H
