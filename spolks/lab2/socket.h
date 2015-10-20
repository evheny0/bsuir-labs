#ifndef SOCKET_H
#define SOCKET_H

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



class Socket
{
    SOCKET socket_object;
  public:
    Socket();
    ~Socket();
    Socket *build_tcp_socket();
    SOCKET get_obj();
};

#endif // SOCKET_H
