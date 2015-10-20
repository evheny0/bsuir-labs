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
    int read_size, current_read_position = 0;
    char client_message[READ_SIZE], buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    while((read_size = recv(client_sock, client_message, READ_SIZE, 0)) > 0) {
        for (unsigned i = 0; i < read_size; ++i) {
            buffer[current_read_position] = client_message[i];
            if (client_message[i] == 10) {
                message_ready(buffer, client_sock);
                current_read_position = 0;
                memset(buffer, 0, BUFFER_SIZE);
            } else {
                current_read_position++;
            }
        }
        memset(client_message, 0, READ_SIZE);
    }
    return read_size;
}

void message_ready(const char *buffer, SOCKET client_sock)
{
    char dbg[] = "Message: ";
    send(client_sock, dbg, strlen(dbg), 0);
    send(client_sock, buffer, strlen(buffer), 0);

    puts(buffer);
}
