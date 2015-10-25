#ifndef CLIENT_H
#define CLIENT_H

#include "basic_socket_handler.h"

const char FILENAME_RECIEVE[100] = "file_to_recieve.jpg";

class Client: public BasicSocketHandler {
    std::fstream _file;
  public:
    Client(const char *ip, int port);
    ~Client();
    void connect_to_server();
    void recieve_file();

  private:
    void send_last_position_to_server(long last_position);
    int get_filesize_from_server();
    void open_file();
};

#endif // CLIENT_H
