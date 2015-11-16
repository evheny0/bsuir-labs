#ifndef CLIENT_H
#define CLIENT_H

#include "basic_socket_handler.h"
#include "transmission_rater.h"

const char FILENAME_RECIEVE[100] = "file_to_recieve.jpg";
const int CYCLES_FOR_PRINT = 100;
extern int is_interrupted;


class Client: public BasicSocketHandler {
    std::fstream _file;
    
  public:
    Client(const char *ip, int port);
    ~Client();
    void connect_to_server();
    void recieve_file();

  private:
    void send_last_position_to_server(long last_position);
    std::ifstream::pos_type get_filesize_from_server();
    void open_file();
    void set_recieving_timeout();
    void do_file_recieve(long long last_position, long long filesize);
};

#endif // CLIENT_H
