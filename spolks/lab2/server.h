#ifndef SERVER_H
#define SERVER_H

#include "basic_socket_handler.h"
#include "client_connection_state.h"
#include <vector>

// const char FILENAME[100] = "file_to_send.txt";
// const char FILENAME[100] = "234.png";
// const char FILENAME[100] = "Fronalpstock_big.jpg";
// const char FILENAME[100] = "ubuntu-14.04-desktop-amd64.iso";
// const char FILENAME[100] = "/home/eugene0/Downloads/Windows 7 PROFESSIONAL Rus x64 Game OS v1.0 by CUTA.iso";
const char FILENAME[100] = "/home/eugene0/Downloads/ubuntu-14.04-desktop-amd64.iso";
extern int is_interrupted;


class Server: public BasicSocketHandler {
    fd_set _socket_set;
    std::string filesize_string;
    std::vector<Socket *> _client_sockets_list;
    std::vector<ClientConnectionState *> _connections_list;

  public:
    Server(const char *ip, int port);
    ~Server();
    void wait_for_connection();
    void send_file();
    void exec();
    
  private:
    void allow_multiple_connections();
    struct sockaddr_in build_server_address();
    void bind_socket_to_address();
    void listen_socket();
    int check_connections();
    void exchange_file_sizes(ClientConnectionState *state);
    void create_new_connection();
    std::ifstream::pos_type get_last_position_from_client();
    void wait_to_client_disconnect();
    void send_filesize_to_client();
    void send_file_to(std::ifstream &file, Socket *socket);
};

#endif // SERVER_H
