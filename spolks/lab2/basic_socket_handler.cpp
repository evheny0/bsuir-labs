#include "basic_socket_handler.h"

BasicSocketHandler::BasicSocketHandler(const char *ip, int port)
{
    _server_addres = build_server_address(ip, port);
}

BasicSocketHandler::~BasicSocketHandler()
{
}

struct sockaddr_in BasicSocketHandler::build_server_address(const char *ip, int port)
{
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);
    return server;
}

void BasicSocketHandler::set_recieving_timeout(Socket *socket)
{
    #ifdef __linux__
    struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    #elif _WIN32
    DWORD timeout = 1;
    #endif

    if (setsockopt(socket->get_obj(), SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
        puts("setsockopt failed");
    }
}

void BasicSocketHandler::recieve_raw_package_from(Socket *from_socket, Package &package, int size, struct sockaddr *src_addr)
{
    int read_size;
    socklen_t addrlen, *addrlen_ptr = NULL;
    // char client_message_raw[BUFFER_MESSAGE_SIZE];
    // package.clear_data();
    if (src_addr) {
        addrlen = sizeof(*src_addr);
        addrlen_ptr = &addrlen;
    }
    read_size = recvfrom(from_socket->get_obj(), package.data, size, 0, src_addr, addrlen_ptr);
    package.size = read_size;

    // why not write to package???
    // if (read_size != -1) {
    //     memcpy(package.data, client_message_raw, read_size);
    // }
}

Package BasicSocketHandler::recieve_package_from(Socket *from_socket)
{
    Package package;
    int read_size, i;
    char client_message_raw[BUFFER_MESSAGE_SIZE];
    read_size = recvfrom(from_socket->get_obj(), client_message_raw, BUFFER_MESSAGE_SIZE + CHECK_CODE_SIZE, MSG_PEEK, NULL, NULL);

    for (i = 0; i < read_size; ++i) {
        if (client_message_raw[i] == '\n') {
            recieve_raw_package_from(from_socket, package, i + 1);
            return package;
        }
    }
}

int BasicSocketHandler::send_package_to(Socket *to_socket, Package &package)
{
    package.data[package.size] = CHECK_CODE[0];
    package.data[package.size + 1] = '\0';
    return sendto(to_socket->get_obj(), package.data, package.size + CHECK_CODE_SIZE, MSG_NOSIGNAL, NULL, NULL);
}

int BasicSocketHandler::send_raw_package_to(Socket *to_socket, Package &package, struct sockaddr *src_addr)
{
    socklen_t addrlen, *addrlen_ptr = NULL;
    if (src_addr) {
        addrlen = sizeof(*src_addr);
        addrlen_ptr = &addrlen;
    }
    return sendto(to_socket->get_obj(), package.data, package.size, MSG_NOSIGNAL, src_addr, addrlen);
}
