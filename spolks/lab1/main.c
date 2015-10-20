#include "main.h"

int main(int argc, char *argv[])
{
    SOCKET client_socket, server_socket;
    int read_size;
    
    server_socket = create_server();
    client_socket = connect_to_client(server_socket);
    read_size = main_echo_cycle(client_socket);
    check_recv_last_response(read_size);
    cleanup(server_socket, client_socket);
    return 0;
}

int main_echo_cycle(SOCKET client_sock)
{
    int read_size;
    char client_message[READ_SIZE], buffer[BUFFER_SIZE], dbg[] = "Message: ";
    memset(buffer, 0, BUFFER_SIZE);

    while((read_size = recv(client_sock, client_message, READ_SIZE, 0)) > 0) {
        // sleep(5);
        sprintf(buffer, "%s%s", buffer, client_message);
        if (client_message[read_size - 1] == '\n') {
            write(client_sock, dbg, strlen(dbg));
            write(client_sock, buffer, strlen(buffer));
            memset(buffer, 0, BUFFER_SIZE);
        } 
        printf("%s", client_message);
        memset(client_message, 0, READ_SIZE);
    }
    return read_size;
}
