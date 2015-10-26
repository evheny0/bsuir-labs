#include "server.h"


Server::Server(const char *ip, int port) : BasicSocketHandler(ip, port)
{
    allow_multiple_connections();
    bind_socket_to_address();
    filesize_string = to_string_fixed(filesize(FILENAME));
    std::cout << " * Filesize: " << filesize_string << "\n";
}

Server::~Server()
{
    std::vector<Socket *>::iterator it;
    for (it = _client_sockets_list.begin(); it != _client_sockets_list.end(); it++) {
        delete *it;
    }
}

void Server::allow_multiple_connections()
{
    int opt = true;
    setsockopt(_socket_ptr->get_obj(), SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));
}

void Server::bind_socket_to_address()
{
    if (bind(_socket_ptr->get_obj(), (struct sockaddr *) &_server_addres, sizeof(_server_addres)) < 0) {
        puts(" - Bind failed");
        exit(1);
    }
    puts(" * Bind done");
}

void Server::exec()
{
    listen_socket();
    check_connections();
    create_new_connection();
    send_file();
}

int Server::check_connections()
{
    int active_count, max_socket = _socket_ptr->get_obj();
    std::vector<Socket *>::iterator it;
    FD_ZERO(&_socket_set);
    FD_SET(_socket_ptr->get_obj(), &_socket_set);

    for (it = _client_sockets_list.begin(); it != _client_sockets_list.end(); it++) {
        FD_SET((*it)->get_obj(), &_socket_set);
        if ((*it)->get_obj() > max_socket) {
            max_socket = (*it)->get_obj();
        }
    }
    active_count = select(max_socket + 1, &_socket_set, NULL, NULL, NULL);
    if (active_count < 0) {
        printf(" * Select error");
    }
    return active_count;
}

void Server::create_new_connection()
{
    struct sockaddr_in client;
    SOCKET sock;
    int sockaddr_in_size = sizeof(struct sockaddr_in);
    if (FD_ISSET(_socket_ptr->get_obj(), &_socket_set)) {
        sock = accept(_socket_ptr->get_obj(), (struct sockaddr *) &client, (socklen_t *) &sockaddr_in_size);
        if (sock < 0) {
            puts(" - Accept failed");
            exit(1);
        }
        _client_sockets_list.push_back(new Socket(sock));
        puts(" * New connection accepted");
    }
}

void Server::listen_socket()
{
    listen(_socket_ptr->get_obj(), SOMAXCONN);
    puts(" * Waiting for incoming connections...");
}

void Server::wait_for_connection()
{
    // SOCKET sock;
    // struct sockaddr_in client;
    // int sockaddr_in_size = sizeof(struct sockaddr_in);

    // sock = accept(_socket_ptr->get_obj(), (struct sockaddr *) &client, (socklen_t *) &sockaddr_in_size);
    // if (sock < 0) {
    //     puts(" - Accept failed");
    //     exit(1);
    // }
    // _client_socket = new Socket(sock);
    // puts(" * Connection accepted");
}

void Server::send_file()
{
    std::ifstream file(FILENAME, std::ios::binary);
    std::ifstream::pos_type last_position = get_last_position_from_client();
    send_filesize_to_client();
    file.seekg(last_position);

    send_file_to(file, _client_sockets_list[0]);

    wait_to_client_disconnect();
    file.close();
}

void Server::exchange_file_sizes(ClientConnectionState *state)
{
    state->open_file(FILENAME);
    std::ifstream::pos_type last_position = get_last_position_from_client();
    state->set_file_position(last_position);
}

void Server::send_file_to(std::ifstream &file, Socket *socket)
{
    char buffer[BUFFER_MESSAGE_SIZE];
    Package package;
    while (!file.eof() && !is_interrupted) {
        file.read(buffer, BUFFER_MESSAGE_SIZE);
        package.size = file.gcount();
        memcpy(package.data, buffer, package.size);
        send_raw_package_to(socket, package);
    }
}

std::ifstream::pos_type Server::get_last_position_from_client()
{
    Package package = recieve_package_from(_client_sockets_list[0]);
    std::cout << " * Last position: " << package.data << "\n";
    return stoll_fixed(package.data);
}

void Server::send_filesize_to_client()
{
    Package package(filesize_string.c_str(), filesize_string.size());
    send_package_to(_client_sockets_list[0], package);
}

void Server::wait_to_client_disconnect()
{
    puts(" * Wait to disconnect");
    char client_message[BUFFER_MESSAGE_SIZE];
    while (recv(_client_sockets_list[0]->get_obj(), client_message, BUFFER_MESSAGE_SIZE, 0));
}
