#include "client_connection_state.h"

ClientConnectionState::ClientConnectionState(Socket *socket)
{
    _socket_ptr = socket;
    _deleted = false;
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
    return _file.gcount();
}
