#include "udp_client.h"

UdpClient::UdpClient(const char *ip, int port) : BasicSocketHandler(ip, port)
{
    _socket_ptr = (new Socket())->build_udp_socket();
    set_recieving_timeout(_socket_ptr);
}

UdpClient::~UdpClient()
{
    _file.close();
    delete _socket_ptr;
}

void UdpClient::recieve_file()
{
    open_file();
    std::ifstream::pos_type last_position = _file.tellp();

    send_last_position_to_server(last_position);
    std::ifstream::pos_type filesize = get_filesize_from_server();

    do_file_recieve(last_position, filesize);
    std::cout << " * File downloaded\n";
}

void UdpClient::open_file()
{
    _file.open(FILENAME_RECIEVE, std::ios::in | std::ios::out | std::ios::binary | std::ofstream::ate);
    if (!_file) {
        _file.open(FILENAME_RECIEVE, std::fstream::binary | std::fstream::trunc | std::fstream::out);
        _file.close();
        _file.open(FILENAME_RECIEVE, std::ios::in | std::ios::out | std::ios::binary | std::ofstream::ate);
    }
}

void UdpClient::do_file_recieve(long long last_position, long long filesize)
{
    long int cycle_counter = 0, recieved_position = 0;
    TransmissionRater rater;
    std::ifstream::pos_type position = last_position;
    while ((last_position < filesize) && !is_interrupted) {
        recieve_raw_package_from(_socket_ptr, _package, UDP_BUFFER_SIZE, &_remote_addr);

        if (_package.size < 1) {
            // lost connection???
            continue;
        }

        memcpy((char *) &recieved_position, _package.data, sizeof(long));

        if (last_position == recieved_position) {
            rater.add_bytes(_package.size);
            _file.write(_package.data + sizeof(long), _package.size - sizeof(long));
            last_position += _package.size - sizeof(long);
        }

        memcpy(_package.data, (char *) &last_position, sizeof(long));
        _package.size = sizeof(long);
        send_raw_package_to(_socket_ptr, _package, &_remote_addr);

        if (!(++cycle_counter % CYCLES_FOR_PRINT)) {
            std::cout << " * Downloaded: " << last_position * 100.0 / filesize << "\% :: ";
            std::cout << rater.get_rate_MBs() << " MB/s\n";
        }
    }
    // filesize++;
    // memcpy(_package.data, (char *) &(filesize), sizeof(long));
    // _package.size = sizeof(long);
    // send_raw_package_to(_socket_ptr, _package, &_remote_addr);
}

void UdpClient::send_last_position_to_server(long last_position)
{
    std::string last_position_str;
    last_position_str = to_string_fixed(last_position);
    std::cout << " * Last position: " << last_position_str << "\n";
    Package package(last_position_str.c_str(), last_position_str.size());
    send_raw_package_to(_socket_ptr, package, (struct sockaddr *) &_server_addres);
    puts("SENT");
}

std::ifstream::pos_type UdpClient::get_filesize_from_server()
{
    recieve_raw_package_from(_socket_ptr, _package, UDP_BUFFER_SIZE, (struct sockaddr *) &_server_addres);
    std::cout << " * Filesize is: " << _package.data << "\n";
    return stoll_fixed(_package.data);
}
