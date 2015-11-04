#include "server.h"


Server::Server(const char *ip, int port) : BasicSocketHandler(ip, port)
{
    _message_buffer = (char *) calloc(BUFFER_MESSAGE_SIZE, sizeof(char));
    filesize_string = to_string_fixed(filesize(FILENAME));
    std::cout << " * Filesize: " << filesize_string << "\n";
    allow_multiple_connections();
    bind_socket_to_address();
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
    size_t size = sizeof(BUFFER_MESSAGE_SIZE);
    setsockopt(_socket_ptr->get_obj(), SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));
    setsockopt(_socket_ptr->get_obj(), SOL_SOCKET, SO_SNDBUF, (char *) &BUFFER_MESSAGE_SIZE, sizeof(BUFFER_MESSAGE_SIZE));
    getsockopt(_socket_ptr->get_obj(), SOL_SOCKET, SO_SNDBUF, (char *) &BUFFER_MESSAGE_SIZE, (socklen_t *) &size);
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
    send_file_to_connected();
    check_connections_absence();
}

TIMEOUT_T Server::make_timeout()
{
    struct timeval timeout = {0, 1};      
}

int Server::check_connections()
{
    int active_count, max_socket = _socket_ptr->get_obj();
    std::vector<ClientConnectionState *>::iterator it;
    FD_ZERO(&_socket_set);
    FD_SET(_socket_ptr->get_obj(), &_socket_set);

    for (it = _connections_list.begin(); it != _connections_list.end(); it++) {
        FD_SET((*it)->get_socket()->get_obj(), &_socket_set);
        if ((*it)->get_socket()->get_obj() > max_socket) {
            max_socket = (*it)->get_socket()->get_obj();
        }
    }

    struct timeval timeout = {0, 1};
    active_count = select(max_socket + 1, &_socket_set, NULL, NULL, &timeout);
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
            return;
        }
        _connections_list.push_back(new ClientConnectionState(new Socket(sock)));
        puts(" * New connection accepted");
        exchange_file_sizes(_connections_list.back());
    }
}

void Server::listen_socket()
{
    listen(_socket_ptr->get_obj(), SOMAXCONN);
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

void Server::exchange_file_sizes(ClientConnectionState *state)
{
    state->open_file(FILENAME);
    std::ifstream::pos_type last_position = get_last_position_from_client(state);
    send_filesize_to_client(state);
    state->set_file_position(last_position);
}

void Server::send_file_to_connected()
{
    ClientConnectionState *link = 0;
    std::vector<ClientConnectionState *>::iterator it;
    std::vector<int> to_delete;
    std::vector<int>::iterator it_to_delete;
    int counter = 0;
    memset(_message_buffer, 0, BUFFER_MESSAGE_SIZE);

    for (it = _connections_list.begin(); it != _connections_list.end(); it++) {
        if (!(*it)->is_eof()) {
            (*it)->read_file(_package.data, BUFFER_MESSAGE_SIZE);
            _package.size = (*it)->file_gcount();
            if (!send_raw_package_to((*it)->get_socket(), _package)) {
                continue;
            }
        } else {
            puts(" * End of file");
            to_delete.push_back(it - _connections_list.begin());
            link = *it;
            delete link;
        }
    }
    for (it_to_delete = to_delete.begin(); it_to_delete != to_delete.end(); it_to_delete++) {
        _connections_list.erase(_connections_list.begin() + *it_to_delete);
    }
}

std::ifstream::pos_type Server::get_last_position_from_client(ClientConnectionState *state)
{
    Package package = recieve_package_from(state->get_socket());
    std::cout << " * Last position: " << package.data << "\n";
    return stoll_fixed(package.data);
}

void Server::send_filesize_to_client(ClientConnectionState *state)
{
    Package package(filesize_string.c_str(), filesize_string.size());
    send_package_to(state->get_socket(), package);
}

void Server::wait_to_client_disconnect()
{
    puts(" * Wait to disconnect");
    char client_message[BUFFER_MESSAGE_SIZE];
    while (recv(_connections_list[0]->get_socket()->get_obj(), client_message, BUFFER_MESSAGE_SIZE, 0));
}

void Server::check_connections_absence()
{
    if (_connections_list.size() == 0) {
        #ifdef __linux__
        sleep(1);
        #elif _WIN32
        Sleep(1000);
        #endif
    }
}
