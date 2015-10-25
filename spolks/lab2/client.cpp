#include "client.h"

Client::Client(const char *ip, int port) : BasicSocketHandler(ip, port)
{
    #ifdef __linux__
    struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    #elif _WIN32
    DWORD timeout = 1;
    #endif

    if (setsockopt(_socket_ptr->get_obj(), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        puts("setsockopt failed");
    }
}

Client::~Client()
{
    _file.close();
}

void Client::connect_to_server()
{
    int result = connect(_socket_ptr->get_obj(), (sockaddr*) &_server_addres, sizeof(_server_addres));
    if (result == -1) {
        puts(" - Can't connect to server");
        exit(1);
    }
    puts(" * Connected to server");
}

void Client::recieve_file()
{
    Package package;
    open_file();
    long last_position = _file.tellp();
    if (last_position == -1) {
        last_position = 0;
    }
    send_last_position_to_server(last_position);
    int filesize = get_filesize_from_server();

    while (last_position < filesize) {
        package = recieve_raw_package_from(_socket_ptr);
        if (package.size == -1) {
            connect_to_server();
            continue;
        }
        _file.write(package.data, package.size);
        last_position += package.size;
        // std::cout << " * Downloaded: " << last_position * 100.0 / filesize << "\% :: " << "last: " << last_position << " total: " << filesize << "\n";
    }
    std::cout << " * Downloaded: 100\%\n";
}

void Client::open_file()
{
    _file.open(FILENAME_RECIEVE, std::ios::in | std::ios::out | std::ios::binary | std::ofstream::ate);
    if(!_file) {
        _file.open(FILENAME_RECIEVE, std::fstream::binary | std::fstream::trunc | std::fstream::out);    
        _file.close();
        _file.open(FILENAME_RECIEVE, std::ios::in | std::ios::out | std::ios::binary | std::ofstream::ate);
    }
}

void Client::send_last_position_to_server(long last_position)
{
    std::string last_position_str;
    last_position_str = to_string_fixed(last_position);
    std::cout << " * Last position: " << last_position_str << "\n";
    Package package(last_position_str.c_str(), last_position_str.size());
    send_package_to(_socket_ptr, package);
}

int Client::get_filesize_from_server()
{
    Package package = recieve_package_from(_socket_ptr);
    std::cout << " * Filesize is: " << package.data << "\n";
    return atoi(package.data);
}
