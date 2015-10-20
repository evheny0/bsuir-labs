#ifndef SOC_LIB_H
#define SOC_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strlen
#include <sys/socket.h>
#include <arpa/inet.h> // inet_addr
#include <unistd.h>    // write


typedef int SOCKET;
const int READ_SIZE = 256;
const int STRING_SIZE = 5000;
const int BUFFER_SIZE = 1024;



SOCKET build_socket();
SOCKET connect_to_client(SOCKET socket_desc);
void assign_address_to_socket(SOCKET socket_desc, struct sockaddr_in server);
void check_recv_last_response(int read_size);
SOCKET create_server();
struct sockaddr_in build_server_address();
void assign_address_to_socket(SOCKET server_socket, struct sockaddr_in server);
void cleanup(SOCKET server_socket, SOCKET client_sock);


#endif // SOC_LIB_H
