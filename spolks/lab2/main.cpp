#include "main.h"

void prepare_socket_lib()
{
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf(" - WSAStartup failed");
        exit(1);
    }
    #endif
}

void on_exit_handler(int signal_type)
{
    if (is_interrupted) {
        puts("Exit on interrupt");
        exit(2);
    }
    is_interrupted = 1;
}

void prepare_exit_signal()
{
    #ifdef __linux__
    struct sigaction signal_handler;
    signal_handler.sa_handler = on_exit_handler;
    sigemptyset(&signal_handler.sa_mask);
    signal_handler.sa_flags = 0;

    sigaction(SIGINT, &signal_handler, NULL);
    #endif
}

void do_server_stuff(const char *ip, int port)
{
    Server server(ip, port);
    server.exec();
}

void do_client_stuff(const char *ip, int port)
{
    Client client(ip, port);
    client.connect_to_server();
    client.recieve_file();
}



int main(int argc, char const *argv[])
{
    prepare_socket_lib();
    prepare_exit_signal();

    if (argc > 3 && !strcmp(argv[1], "server")) {
        do_server_stuff(argv[2], atoi(argv[3]));
    } else if (argc > 3 && !strcmp(argv[1], "client")) {
        do_client_stuff(argv[2], atoi(argv[3]));
    }

    return 0;
}
