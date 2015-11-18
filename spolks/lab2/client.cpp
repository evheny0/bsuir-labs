#include "client.h"

Client::Client(const char *ip, int port) : BasicSocketHandler(ip, port)
{
    _socket_ptr = (new Socket())->build_tcp_socket();
    set_recieving_timeout(_socket_ptr);
}

Client::~Client()
{
    _file.close();
    delete _socket_ptr;
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
    open_file();
    std::ifstream::pos_type last_position = _file.tellp();
    // wtf???
    if (last_position == -1) {
        last_position = 0;
    }
    send_last_position_to_server(last_position);
    std::ifstream::pos_type filesize = get_filesize_from_server();

    do_file_recieve(last_position, filesize);
    std::cout << " * File downloaded\n";
}

void Client::do_file_recieve(long long last_position, long long filesize)
{
    int cycle_counter = 0;
    Package package;
    TransmissionRater rater;
    while ((last_position < filesize) && !is_interrupted) {
        recieve_raw_package_from(_socket_ptr, package);
        if (package.size <= 0) {
            connect_to_server();
            continue;
        }
        rater.add_bytes(package.size);
        _file.write(package.data, package.size);
        last_position += package.size;
        if (!(++cycle_counter % CYCLES_FOR_PRINT)) {
            std::cout << " * Downloaded: " << last_position * 100.0 / filesize << "\% :: ";
            std::cout << rater.get_rate_MBs() << " MB/s\n";
        }
    }
    // package.free();
}

void Client::open_file()
{
    using ios = std::ios;
    _file.open(FILENAME_RECIEVE, ios::in | ios::out | ios::binary | ios::ate);
    if (!_file) {
        _file.open(FILENAME_RECIEVE, ios::binary | ios::trunc | ios::out);
        _file.close();
        _file.open(FILENAME_RECIEVE, ios::in | ios::out | ios::binary | ios::ate);
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

std::ifstream::pos_type Client::get_filesize_from_server()
{
    Package package = recieve_package_from(_socket_ptr);
    std::cout << " * Filesize is: " << package.data << "\n";
    return stoll_fixed(package.data);
}
