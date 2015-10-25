#include "basic_socket_handler.h"

BasicSocketHandler::BasicSocketHandler(const char *ip, int port)
{
    _socket_ptr = (new Socket())->build_tcp_socket();
    _server_addres = build_server_address(ip, port);
}

BasicSocketHandler::~BasicSocketHandler()
{
    delete _socket_ptr;
}

struct sockaddr_in BasicSocketHandler::build_server_address(const char *ip, int port)
{
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);
    return server;
}

Package BasicSocketHandler::recieve_raw_package_from(Socket *from_socket, int size)
{
    int read_size;
    Package package;
    char client_message_raw[BUFFER_MESSAGE_SIZE];

    read_size = recv(from_socket->get_obj(), client_message_raw, size, 0);
    package.size = read_size;
    if (read_size != -1) {
        memcpy(package.data, client_message_raw, read_size);
    }
    return package;
}

Package BasicSocketHandler::recieve_package_from(Socket *from_socket)
{
    int read_size, i;
    char client_message_raw[BUFFER_MESSAGE_SIZE];
    read_size = recv(from_socket->get_obj(), client_message_raw, BUFFER_MESSAGE_SIZE + CHECK_CODE_SIZE, MSG_PEEK);

    for (i = 0; i < read_size; ++i) {
        if (client_message_raw[i] == '\n') {
            return recieve_raw_package_from(from_socket, i + 1);
        }
    }
}

int BasicSocketHandler::send_package_to(Socket *to_socket, Package package)
{
    package.data[package.size] = CHECK_CODE[0];
    package.data[package.size + 1] = '\0';
    return send(to_socket->get_obj(), package.data, package.size + CHECK_CODE_SIZE, 0);
}

int BasicSocketHandler::send_raw_package_to(Socket *to_socket, Package package)
{
    return send(to_socket->get_obj(), package.data, package.size, 0);
}
