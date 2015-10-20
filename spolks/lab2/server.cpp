#include "server.h"


Server::Server()
{
    _socket_ptr = (new Socket())->build_tcp_socket();
    _server_addres = build_server_address();
    assign_address_to_socket();
}

Server::~Server()
{
    delete _socket_ptr;
}



struct sockaddr_in Server::build_server_address()
{
    //Prepare the sockaddr_in structure
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8889);
    return server;
}

void Server::assign_address_to_socket()
{
    if (bind(_socket_ptr->get_obj(), (struct sockaddr *) &_server_addres, sizeof(_server_addres)) < 0) {
        puts("Bind failed");
        exit(1);
    }
    puts("bind done");
}
