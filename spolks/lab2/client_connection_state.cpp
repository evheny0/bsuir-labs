#include "client_connection_state.h"

ClientConnectionState::ClientConnectionState(Socket *socket)
{
    _socket_ptr = socket;
    _deleted = false;
    _last_read = 0;
}

ClientConnectionState::~ClientConnectionState()
{
    _file.close();
    puts(" * File closed");
    delete _socket_ptr;
}

Socket *ClientConnectionState::get_socket()
{
    return _socket_ptr;
}

SOCKET ClientConnectionState::get_socket_obj()
{
    return _socket_ptr->get_obj();
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

void ClientConnectionState::set_deleted(bool value)
{
    _deleted = value;
}

bool ClientConnectionState::is_deleted()
{
    return _deleted;
}

bool ClientConnectionState::is_eof()
{
    return _file.eof();
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
