#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "basic_socket_handler.h"
#include "transmission_rater.h"

extern int is_interrupted;



class UdpClient: public BasicSocketHandler {
    std::fstream _file;
    Socket *_socket_ptr;
    struct sockaddr _remote_addr;

  public:
    UdpClient(const char *ip, int port);
    ~UdpClient();
    void recieve_file();
    void open_file();
    void do_file_recieve(long long last_position, long long filesize);
    void send_last_position_to_server(long last_position);
    std::ifstream::pos_type get_filesize_from_server();

};

#endif // UDP_CLIENT_H
