#ifndef SERVER_H
#define SERVER_H

#include "basic_socket_handler.h"

// const char FILENAME[100] = "file_to_send.txt";
const char FILENAME[100] = "234.png";

class Server: public BasicSocketHandler {
    Socket *_client_socket;
  public:
    Server(const char *ip, int port);
    ~Server();
    void wait_for_connection();
    void send_file();
    
  private:
    struct sockaddr_in build_server_address();
    void bind_socket_to_address();
    int get_last_position_from_client();
    void wait_to_client_disconnect();
    void send_filesize_to_client();
};

#endif // SERVER_H
