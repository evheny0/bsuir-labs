#ifndef CLIENT_H
#define CLIENT_H

#include "basic_socket_handler.h"

class Client: public BasicSocketHandler {
    
  public:
    Client(const char *ip, int port);
    ~Client();
    void connect_to_server();
    void recieve_file();

  private:
    void send_last_position_to_server(long last_position);
    void send_end_to_server();
    int get_filesize_from_server();
};

#endif // CLIENT_H
