#include "client.h"

Client::Client(const char *ip, int port) : BasicSocketHandler(ip, port)
{

}

Client::~Client()
{
    
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
    std::fstream file("file_to_recieve", std::fstream::out | std::fstream::in | std::ofstream::ate);
    long last_position = file.tellp();
    send_last_position_to_server(last_position);
    int filesize = get_filesize_from_server();

    while (last_position < filesize) {
        package = recieve_package_from(_socket_ptr);
        file.write(package.data.c_str(), package.size);
        last_position += package.size;
        std::cout << " * Downloaded: " << last_position * 100.0 / filesize << "\%\n";
    }
    std::cout << " * Downloaded: 100\%\n";

    file.close();
}

void Client::send_last_position_to_server(long last_position)
{
    std::string last_position_str;
    if (last_position == -1) {
        last_position_str = to_string_fixed(0);
    } else {
        last_position_str = to_string_fixed(last_position);
    }

    Package package(last_position_str);
    send_package_to(_socket_ptr, package);
}

int Client::get_filesize_from_server()
{
    Package package = recieve_package_from(_socket_ptr);
    std::cout << " * Filesize is: " << package.data << "\n";
    return atoi(package.data.c_str());
}

void Client::send_end_to_server()
{
    // no longer needed
    std::string str("\0");
    Package package(str, 1);
    send_package_to(_socket_ptr, package);
}
