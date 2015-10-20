#include "soc_lib.h"


SOCKET build_socket()
{
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed");
        exit(1);
    }
    #endif

    //Create socket               sok type, protocol,   
    SOCKET server_socket = socket(AF_INET , SOCK_STREAM, 0);
    if (server_socket == -1) {
        puts("Could not create socket");
    } else {
        puts("Socket created");
    }
    return server_socket;
}

SOCKET create_server()
{
    SOCKET server_socket = build_socket();
    struct sockaddr_in server = build_server_address();
    assign_address_to_socket(server_socket, server);
    return server_socket;
}

struct sockaddr_in build_server_address()
{
    //Prepare the sockaddr_in structure
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8889);
    return server;
}

void assign_address_to_socket(SOCKET server_socket, struct sockaddr_in server)
{
    if (bind(server_socket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        puts("Bind failed");
        exit(1);
    }
    puts("bind done");
}

SOCKET connect_to_client(SOCKET server_socket)
{
    SOCKET client_sock;
    struct sockaddr_in client;
    // will be used to accept incoming connection requests using accept()
    listen(server_socket, SOMAXCONN);

    puts("Waiting for incoming connections...");
    int sockaddr_in_size = sizeof(struct sockaddr_in);

    // accept connection from an incoming client
    client_sock = accept(server_socket, (struct sockaddr *) &client, (socklen_t*) &sockaddr_in_size);
    if (client_sock < 0) {
        puts("Accept failed");
        exit(1);
    }
    puts("Connection accepted");
    return client_sock;
}

void check_recv_last_response(int read_size)
{
    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        puts("recv failed");
    }
}

void cleanup(SOCKET server_socket, SOCKET client_sock)
{
    #ifdef _WIN32
    shutdown(client_sock, SD_SEND);
    closesocket(server_socket);
    closesocket(client_sock);
    #elif __linux__
    close(server_socket);
    close(client_sock);
    #endif
}
