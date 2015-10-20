#include "socket.h"


Socket::Socket()
{
}

Socket *Socket::build_tcp_socket()
{
    socket_object = socket(AF_INET , SOCK_STREAM, 0);
    if (socket_object == -1) {
        puts("Could not create socket");
    } else {
        puts("Socket created");
    }
    return this;
}

Socket::~Socket()
{
    if (socket_object != -1) {
        #ifdef _WIN32
        shutdown(socket_object, SD_SEND);
        closesocket(socket_object);
        #elif __linux__
        close(socket_object);
        #endif
    }
}

SOCKET Socket::get_obj()
{
    return socket_object;
}
