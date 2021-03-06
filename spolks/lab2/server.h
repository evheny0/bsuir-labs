#ifndef SERVER_H
#define SERVER_H

#include "basic_socket_handler.h"
#include "client_connection_state.h"
#include <vector>
#include <list>
#include <algorithm>

#ifdef __linux__
typedef struct timeval TIMEOUT_T;      
#elif _WIN32
typedef DWORD TIMEOUT_T;
#endif

// d02a2301576c4b8bd35ce7833649066dc52dad41
// const char FILENAME[100] = "file_to_send.txt";
// const char FILENAME[100] = "234.png";
// const char FILENAME[100] = "Fronalpstock_big.jpg";
// const char FILENAME[100] = "ubuntu-14.04-desktop-amd64.iso";
// const char FILENAME[100] = "/home/eugene0/Downloads/Windows 7 PROFESSIONAL Rus x64 Game OS v1.0 by CUTA.iso";
// d4d44272ee5f5bf887a9c85ad09ae957bc55f89d
const char FILENAME[100] = "/home/eugene0/Downloads/ubuntu-14.04-desktop-amd64.iso";
// const char FILENAME[100] = "1.lalala";

extern int is_interrupted;
const int CYCLES_TO_CHECK_CONNECTIONS = 5;


class Server: public BasicSocketHandler {
    fd_set _socket_set;
    Socket *_udp_socket_ptr;
    Socket *_tcp_socket_ptr;
    std::string filesize_string;
    std::vector<Socket *> _client_sockets_list;
    std::vector<ClientConnectionState *> _connections_list;
    std::vector<int> _connection_id_to_delete;

  public:
    Server(const char *ip, int port);
    ~Server();
    void exec();
    
  private:
    void allow_multiple_connections();
    struct sockaddr_in build_server_address();
    void bind_socket_to_address(Socket *socket);
    void listen_tcp_socket();
    int check_connections();
    void exchange_file_sizes(ClientConnectionState *state);
    void udp_exchange_file_sizes(ClientConnectionState *state);
    void create_new_connection();
    std::ifstream::pos_type get_last_position_from_client(ClientConnectionState *state);
    void wait_to_client_disconnect();
    void send_filesize_to_client(ClientConnectionState *state);
    void send_filepart_by_tcp(ClientConnectionState *state);
    void send_filepart_by_udp(ClientConnectionState *state);
    void send_file_to_connected();
    void check_connections_absence();
    TIMEOUT_T make_timeout();
    void delete_disconnected();

};

#endif // SERVER_H
