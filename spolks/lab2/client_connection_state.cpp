#include "client_connection_state.h"

ClientConnectionState::ClientConnectionState(Socket *socket)
{
    _socket_ptr = socket;
    _last_read = 0;
    _connection_type = TCP_CONNECTION;
    _closed = false;
}

ClientConnectionState::ClientConnectionState()
{
    _socket_ptr = NULL;
    _last_read = 0;
    _connection_type = UDP_CONNECTION;
    _closed = false;
}

ClientConnectionState::~ClientConnectionState()
{
    _file.close();
    puts(" * File closed");
    if (_socket_ptr) {
        delete _socket_ptr;
    }
}

Socket *ClientConnectionState::get_socket()
{
    return _socket_ptr;
}

SOCKET ClientConnectionState::get_socket_obj()
{
    if (_socket_ptr) {
        return _socket_ptr->get_obj();
    } else {
        return 0;
    }
}

void ClientConnectionState::open_file(const char *str)
{
    _file.open(str, std::ios::binary);
}

std::ifstream &ClientConnectionState::get_file()
{
    return _file;
}

void ClientConnectionState::set_file_position(std::ifstream::pos_type &position)
{
    _file.seekg(position);
}

void ClientConnectionState::revert_last_file_read()
{
    _file.seekg(-_last_read, std::ios_base::cur);
    _last_read = 0;
}

bool ClientConnectionState::is_eof()
{
    return _file.eof();
}

bool ClientConnectionState::is_closed()
{
    return is_eof() || _closed;
}

void ClientConnectionState::set_closed()
{
    _closed = true;
}

void ClientConnectionState::read_file(char *buffer, long size)
{
    _file.read(buffer, size);
}

long ClientConnectionState::file_gcount()
{
    _last_read = _file.gcount();
    return _last_read;
}

long ClientConnectionState::last_position()
{
    return _file.tellg();
}

struct sockaddr *ClientConnectionState::get_sockaddr()
{
    return &_remote_addr;
}

bool ClientConnectionState::is_tcp()
{
    return _connection_type == TCP_CONNECTION;
}

bool ClientConnectionState::is_udp()
{
    return _connection_type == UDP_CONNECTION;
}
