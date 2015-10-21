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

Package BasicSocketHandler::recieve_package_from(Socket *from_socket)
{
    Package package;
    int position, read_size;
    char client_message[BUFFER_MESSAGE_SIZE];

    // if (_last_package.size && (_last_package.last_position != _last_package.size + CHECK_CODE_SIZE)) {
    //     position = _last_package.data.find(CHECK_CODE, _last_package.last_position);
    //     if (position == std::string::npos) {
    //         package.data = _last_package.data.substr(_last_package.last_position);
    //         package.size = data.size();
    //     } else {
    //         _last_package.last_position = position;
    //         package.data = _last_package.data.substr(_last_package.last_position, position);
    //         package.size = data.size();
    //         return package;
    //     }
    // }
    read_size = recv(from_socket->get_obj(), client_message, BUFFER_MESSAGE_SIZE + CHECK_CODE_SIZE, 0);
    
    // position = client_message.find(CHECK_CODE, _last_package.last_position);

    // if (position == std::string::npos) {
        
    // }

    package.size += read_size;
    package.data += client_message;
    return package;
}

int BasicSocketHandler::send_package_to(Socket *to_socket, Package package)
{
    package.data += CHECK_CODE;
    return send(to_socket->get_obj(), package.data.c_str(), package.size + CHECK_CODE_SIZE, 0);
}
