#ifndef SOC_LIB_H
#define SOC_LIB_H


#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strlen
#include <sys/socket.h>
#include <arpa/inet.h> // inet_addr
#include <unistd.h>    // write
typedef int SOCKET;

#elif _WIN32

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#undef UNICODE
#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#endif



SOCKET build_socket();
SOCKET connect_to_client(SOCKET socket_desc);
void assign_address_to_socket(SOCKET socket_desc, struct sockaddr_in server);
void check_recv_last_response(int read_size);
SOCKET create_server();
struct sockaddr_in build_server_address();
void assign_address_to_socket(SOCKET server_socket, struct sockaddr_in server);
void cleanup(SOCKET server_socket, SOCKET client_sock);


#endif // SOC_LIB_H
