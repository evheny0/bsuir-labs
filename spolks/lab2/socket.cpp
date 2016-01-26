#include "socket.h"


Socket::Socket()
{
}

Socket::Socket(SOCKET initial_socket)
{
    _socket_object = initial_socket;
}

Socket *Socket::build_tcp_socket()
{
    _socket_object = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_object == -1) {
        puts(" * Could not create socket");
    } else {
        puts(" * Socket created");
    }
    return this;
}

Socket *Socket::build_udp_socket()
{
    _socket_object = socket(AF_INET, SOCK_DGRAM, 0);
    if (_socket_object == -1) {
        puts(" * Could not create socket");
    } else {
        puts(" * Socket created");
    }
    return this;
}

Socket::~Socket()
{
    if (_socket_object != -1) {
        shutdown(_socket_object, SHUT_ALL);
        
        #ifdef _WIN32
        closesocket(_socket_object);
        #elif __linux__
        puts(" * socket closed");
        close(_socket_object);
        #endif
    }
}

SOCKET Socket::get_obj()
{
    return _socket_object;
}
