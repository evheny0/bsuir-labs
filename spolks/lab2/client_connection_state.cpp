#include "client_connection_state.h"

ClientConnectionState::ClientConnectionState(Socket *socket)
{
    _socket_ptr = socket;
}

ClientConnectionState::~ClientConnectionState()
{
    _file.close();
    delete _socket_ptr;
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
