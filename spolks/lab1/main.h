#ifndef MAIN_H
#define MAIN_H

#include "soc_lib.h"

const int READ_SIZE = 256;
const int STRING_SIZE = 5000;
const int BUFFER_SIZE = 1024;

int main_echo_cycle(SOCKET client_sock);
void message_ready(const char *buffer, SOCKET client_sock);
void process_message_and_send(char *client_message, int read_size, char *buffer, int current_read_position, SOCKET client_sock);


#endif // MAIN_H
