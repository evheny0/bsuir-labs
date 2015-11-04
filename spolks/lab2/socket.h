#ifndef SOCKET_H
#define SOCKET_H

#ifdef __linux__

#include <string.h>    // strlen
#include <sys/socket.h>
#include <arpa/inet.h> // inet_addr
#include <unistd.h>    // write
#define SHUT_ALL SHUT_RDWR
typedef int SOCKET;

#elif _WIN32

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#undef UNICODE
#include <conio.h>
#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#define SHUT_ALL SD_BOTH

#endif

#include "package.h"




class Socket {
    SOCKET socket_object;
  public:
    Socket();
    Socket(SOCKET initial_socket);
    ~Socket();
    Socket *build_tcp_socket();
    SOCKET get_obj();
};

#endif // SOCKET_H
