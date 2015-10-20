#include "basic_socket_handler.h"

BasicSocketHandler::BasicSocketHandler(char *ip, int port)
{
    _socket_ptr = (new Socket())->build_tcp_socket();
    _server_addres = build_server_address(ip, port);
}

BasicSocketHandler::~BasicSocketHandler()
{
    delete _socket_ptr;
}

struct sockaddr_in BasicSocketHandler::build_server_address(char *ip, int port)
{
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);
    return server;
}
