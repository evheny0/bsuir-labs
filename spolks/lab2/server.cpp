#include "server.h"


Server::Server(const char *ip, int port) : BasicSocketHandler(ip, port)
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
        puts(" - Bind failed");
        exit(1);
    }
    puts(" * Bind done");
}

void Server::wait_for_connection()
{
    SOCKET sock;
    struct sockaddr_in client;
    int sockaddr_in_size = sizeof(struct sockaddr_in);

    listen(_socket_ptr->get_obj(), SOMAXCONN);
    puts(" * Waiting for incoming connections...");

    sock = accept(_socket_ptr->get_obj(), (struct sockaddr *) &client, (socklen_t *) &sockaddr_in_size);
    if (sock < 0) {
        puts(" - Accept failed");
        exit(1);
    }
    _client_socket = new Socket(sock);
    puts(" * Connection accepted");
}

void Server::send_file()
{
    char buffer[BUFFER_MESSAGE_SIZE];
    Package package;
    std::ifstream file("file_to_send.txt", std::ios::binary);
    int last_position = get_last_position_from_client();
    send_filesize_to_client();

    file.seekg(last_position);

    while (!file.eof()) {
        file.read(buffer, BUFFER_MESSAGE_SIZE);
        package.size = file.gcount();
        std::cout << "Data send " << package.size << "\n";
        buffer[package.size] = '\0';
        package.data = buffer;
        send_raw_package_to(_client_socket, package);
    }

    wait_to_client_disconnect();
    file.close();
}

int Server::get_last_position_from_client()
{
    Package package = recieve_package_from(_client_socket);
    std::cout << " * Last position: " << package.data << "\n";
    int position = atoi(package.data.c_str());
    return position;
}

void Server::send_filesize_to_client()
{
    std::string filesize_string;
    filesize_string = to_string_fixed((int) filesize("Fronalpstock_big.jpg"));
    std::cout << " * Filesize: " << filesize_string << "\n";
    Package package(filesize_string);
    send_package_to(_client_socket, package);
}

void Server::wait_to_client_disconnect()
{
    puts(" * Wait to disconnect");
    char client_message[BUFFER_MESSAGE_SIZE];
    while (recv(_client_socket->get_obj(), client_message, BUFFER_MESSAGE_SIZE, 0));
}
