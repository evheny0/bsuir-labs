#include "server.h"


Server::Server(const char *ip, int port) : BasicSocketHandler(ip, port)
{
    filesize_string = to_string_fixed(filesize(FILENAME));
    std::cout << " * Filesize: " << filesize_string << "\n";
    _udp_socket_ptr = (new Socket())->build_udp_socket();
    _tcp_socket_ptr = (new Socket())->build_tcp_socket();
    allow_multiple_connections();
    bind_socket_to_address(_udp_socket_ptr);
    bind_socket_to_address(_tcp_socket_ptr);
    listen_tcp_socket();
}

Server::~Server()
{
    std::vector<Socket *>::iterator it;
    for (it = _client_sockets_list.begin(); it != _client_sockets_list.end(); it++) {
        delete *it;
    }
    delete _udp_socket_ptr;
    delete _tcp_socket_ptr;
}

void Server::allow_multiple_connections()
{
    int opt = true;
    size_t size = sizeof(BUFFER_MESSAGE_SIZE);
    setsockopt(_tcp_socket_ptr->get_obj(), SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));
    setsockopt(_tcp_socket_ptr->get_obj(), SOL_SOCKET, SO_SNDBUF, (char *) &BUFFER_MESSAGE_SIZE, sizeof(BUFFER_MESSAGE_SIZE));
    getsockopt(_tcp_socket_ptr->get_obj(), SOL_SOCKET, SO_SNDBUF, (char *) &BUFFER_MESSAGE_SIZE, (socklen_t *) &size);
}

void Server::bind_socket_to_address(Socket *socket)
{
    if (bind(socket->get_obj(), (struct sockaddr *) &_server_addres, sizeof(_server_addres)) < 0) {
        puts(" - Bind failed");
        exit(1);
    }
    puts(" * Bind done");
}

void Server::exec()
{
    while (!is_interrupted) {
        check_connections();
        create_new_connection();
        send_file_to_connected();
        delete_disconnected();
        check_connections_absence();
    }
}

TIMEOUT_T Server::make_timeout()
{
    struct timeval timeout = {0, 1};      
}

int Server::check_connections()
{
    int active_count, max_socket = _tcp_socket_ptr->get_obj();
    std::vector<ClientConnectionState *>::iterator it;
    FD_ZERO(&_socket_set);
    FD_SET(_tcp_socket_ptr->get_obj(), &_socket_set);
    FD_SET(_udp_socket_ptr->get_obj(), &_socket_set);

    for (it = _connections_list.begin(); it != _connections_list.end(); it++) {
        FD_SET((*it)->get_socket_obj(), &_socket_set);
        if ((*it)->get_socket_obj() > max_socket) {
            max_socket = (*it)->get_socket_obj();
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
    if (FD_ISSET(_tcp_socket_ptr->get_obj(), &_socket_set)) {
        sock = accept(_tcp_socket_ptr->get_obj(), (struct sockaddr *) &client, (socklen_t *) &sockaddr_in_size);
        if (sock < 0) {
            puts(" - Accept failed");
            return;
        }
        _connections_list.push_back(new ClientConnectionState(new Socket(sock)));
        puts(" * New connection accepted");
        exchange_file_sizes(_connections_list.back());
    }
    if (FD_ISSET(_udp_socket_ptr->get_obj(), &_socket_set)) {
        _connections_list.push_back(new ClientConnectionState(new Socket(sock)));
        puts(" * New connection accepted");
        exchange_file_sizes(_connections_list.back());
    }
}

void Server::listen_tcp_socket()
{
    listen(_tcp_socket_ptr->get_obj(), SOMAXCONN);
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

    for (it = _connections_list.begin(); it != _connections_list.end(); it++) {
        if (!(*it)->is_eof()) {
            (*it)->read_file(_package.data, BUFFER_MESSAGE_SIZE);
            _package.size = (*it)->file_gcount();
            if (!send_raw_package_to((*it)->get_socket(), _package)) {
                (*it)->revert_last_file_read();
                continue;
            }
        } else {
            puts(" * End of file");
            _connection_id_to_delete.push_back(it - _connections_list.begin());
            link = *it;
            delete link;
        }
    }
}

void Server::delete_disconnected()
{
    std::vector<int>::iterator it_to_delete = _connection_id_to_delete.begin();
    for ( ; it_to_delete != _connection_id_to_delete.end(); it_to_delete++) {
        _connections_list.erase(_connections_list.begin() + *it_to_delete);
    }
    _connection_id_to_delete.clear();
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
    while (recv(_connections_list[0]->get_socket_obj(), client_message, BUFFER_MESSAGE_SIZE, 0));
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
