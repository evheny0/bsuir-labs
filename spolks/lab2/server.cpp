#include "server.h"


Server::Server(char *ip, int port) : BasicSocketHandler(ip, port)
{
    bind_socket_to_address();
    _client_socket = 0;
}

Server::~Server()
{
    if (_client_socket != 0) {
        delete _client_socket;
    }
}

void Server::bind_socket_to_address()
{
    if (bind(_socket_ptr->get_obj(), (struct sockaddr *) &_server_addres, sizeof(_server_addres)) < 0) {
        puts("Bind failed");
        exit(1);
    }
    puts("bind done");
}

void Server::wait_for_connection()
{
    SOCKET sock;
    struct sockaddr_in client;
    int sockaddr_in_size = sizeof(struct sockaddr_in);

    listen(_socket_ptr->get_obj(), SOMAXCONN);
    puts("Waiting for incoming connections...");

    sock = accept(_socket_ptr->get_obj(), (struct sockaddr *) &client, (socklen_t *) &sockaddr_in_size);
    if (sock < 0) {
        puts("Accept failed");
        exit(1);
    }
    _client_socket = new Socket(sock);
    puts("Connection accepted");
}
