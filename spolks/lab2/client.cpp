#include "client.h"

Client::Client(const char *ip, int port) : BasicSocketHandler(ip, port)
{
    set_recieving_timeout();
}

Client::~Client()
{
    _file.close();
}

void Client::set_recieving_timeout()
{
    #ifdef __linux__
    struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    #elif _WIN32
    DWORD timeout = 1;
    #endif

    if (setsockopt(_socket_ptr->get_obj(), SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
        puts("setsockopt failed");
    }
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
    if (last_position == -1) {
        last_position = 0;
    }
    send_last_position_to_server(last_position);
    std::ifstream::pos_type filesize = get_filesize_from_server();
    // preallocate_file(filesize, last_position);

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
        if (package.size == -1) {
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
    package.free();
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

void Client::preallocate_file(std::ifstream::pos_type &size, std::ifstream::pos_type &last_position)
{
    _file.seekg(size);
    // fallocate()
    _file.write("\0", sizeof("\0"));
    _file.close();
    _file.open(FILENAME_RECIEVE, std::ios::in | std::ios::out | std::ios::binary | std::ofstream::ate);
    _file.seekg(last_position);
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
