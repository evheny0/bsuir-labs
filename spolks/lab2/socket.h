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

#include <stdio.h>
#include <string>
#include <stdlib.h>

static int BUFFER_MESSAGE_SIZE = 1000000;


struct Package {
    char *data;
    long size;
    bool delete_link;

    Package()
    {
        data = new char[BUFFER_MESSAGE_SIZE]();
        // memset(data, 0, BUFFER_MESSAGE_SIZE);
        size = 0;
        delete_link = false;
    }
    Package(const char *package_data, int new_size)
    {
        data = new char[BUFFER_MESSAGE_SIZE]();
        // memset(data, 0, BUFFER_MESSAGE_SIZE);
        memcpy(data, package_data, new_size);
        size = new_size;
        delete_link = false;
    }
    ~Package()
    {
        // delete data;
    }
    void free()
    {
        delete data;
    }
    void clear_data()
    {
        memset(data, 0, BUFFER_MESSAGE_SIZE);
    }
    // Package(std::string package_data)
    // {
    //     data = package_data;
    //     size = data.size();
    //     last_position = 0;
    // }
    // Package(std::string package_data, int package_size)
    // {
    //     data = package_data;
    //     size = package_size;
    //     last_position = 0;
    // }
};

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
