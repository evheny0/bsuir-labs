#include "main.h"

void prepare_socket()
{
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf(" - WSAStartup failed");
        exit(1);
    }
    #endif
}

int main(int argc, char const *argv[])
{
    prepare_socket();
    if (argc > 1 && !strcmp(argv[1], "server")) {
        Server server("127.0.0.1", 8889);
        server.wait_for_connection();
        server.send_file();
    } else if (argc > 1 && !strcmp(argv[1], "client")) {
        Client client("127.0.0.1", 8889);
        client.connect_to_server();
        client.recieve_file();
    }

    return 0;
}
