#include "basic_socket_handler.h"

BasicSocketHandler::BasicSocketHandler(const char *ip, int port)
{
    _socket_ptr = (new Socket())->build_tcp_socket();
    _server_addres = build_server_address(ip, port);
    _last_data = "";
    _last_data_size = 0;
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

Package BasicSocketHandler::recieve_raw_package_from(Socket *from_socket)
{
    int read_size;
    Package package;
    char client_message_raw[BUFFER_MESSAGE_SIZE];

    if (_last_data.size() > 0) {
        package.data = _last_data;
        package.size = _last_data_size;
        _last_data = "";
        return package;
    }
    read_size = recv(from_socket->get_obj(), client_message_raw, BUFFER_MESSAGE_SIZE, 0);
    package.size = read_size;
    package.data.resize(read_size);
    std::cout << client_message_raw << "\n";
    package.data = client_message_raw;
    return package;
}

Package BasicSocketHandler::recieve_package_from(Socket *from_socket)
{
    Package package, recieved_package;
    int position, read_size;
    char client_message_raw[BUFFER_MESSAGE_SIZE];
    std::string client_message;

    // if (_last_package.size && (_last_package.last_position != _last_package.size + CHECK_CODE_SIZE)) {
    //     position = find_position(_last_package.data, CHECK_CODE, _last_package.last_position);
    //     if (position == std::string::npos) {
    //         package.data = _last_package.data.substr(_last_package.last_position);
    //         package.size = package.data.size();
    //     } else {
    //         _last_package.last_position = position;
    //         package.data = _last_package.data.substr(_last_package.last_position, position);
    //         package.size = package.data.size();
    //         return package;
    //     }
    // } else {
    //     _last_package.last_position = 0;
    // }

    if (_last_data.size() > 0) {
        position = find_position(_last_data, CHECK_CODE);
        if (position == std::string::npos) {
            package.data = _last_data;
            package.size = package.data.size();
            _last_data = "";
            _last_data_size = 0;
        } else {
            package.data = _last_data.substr(0, position);
            package.size = package.data.size();
            _last_data = _last_data.substr(position);
            _last_data_size -= position;
            return package;
        }
    }

    do {
        read_size = recv(from_socket->get_obj(), client_message_raw, BUFFER_MESSAGE_SIZE + CHECK_CODE_SIZE, 0);
        client_message = client_message_raw;

        position = find_position(client_message, CHECK_CODE);

        if (position == std::string::npos) {
            package.data += client_message.substr(0);
            package.size = package.data.size();
            continue;
        } else {
            package.data += client_message.substr(0, position);
            package.size = package.data.size();
            _last_data = client_message.substr(position + 2, read_size);
            _last_data_size = read_size - position - 2;
        }
    } while (false);
    return package;

    // do {
    //     read_size = recv(from_socket->get_obj(), client_message_raw, BUFFER_MESSAGE_SIZE + CHECK_CODE_SIZE, 0);
    //     client_message = client_message_raw;
    //     std::cout << "All message: " << client_message << "\n";

    //     position = find_position(client_message, CHECK_CODE, _last_package.last_position);

    //     if (position == std::string::npos) {
    //         package.data += client_message.substr(0);
    //         package.size = package.data.size();
    //         continue;
    //     } else {
    //         package.data += client_message.substr(0, position);
    //         std::cout << position << "\n";

    //         package.size = package.data.size();
    //         _last_package.data = client_message;
    //         _last_package.size = client_message.size();
    //         _last_package.last_position = position;
    //     }
    // } while (false);

    // // package.size += read_size;
    // // package.data += client_message;
    // return package;
}

int BasicSocketHandler::send_package_to(Socket *to_socket, Package package)
{
    package.data += CHECK_CODE;
    return send(to_socket->get_obj(), package.data.c_str(), package.size + CHECK_CODE_SIZE, 0);
}

int BasicSocketHandler::send_raw_package_to(Socket *to_socket, Package package)
{
    return send(to_socket->get_obj(), package.data.c_str(), package.size, 0);
}
