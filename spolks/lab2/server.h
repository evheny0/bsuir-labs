#ifndef SERVER_H
#define SERVER_H

#include "basic_socket_handler.h"

class Server: public BasicSocketHandler {
    Socket *_client_socket;
  public:
    Server(char *ip, int port);
    ~Server();
    void wait_for_connection();
    
  private:
    struct sockaddr_in build_server_address();
    void bind_socket_to_address();
};

#endif // SERVER_H
