#include "main.h"


int main(int argc, char const *argv[])
{
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed");
        exit(1);
    }
    #endif

    Server server("127.0.0.1", 8889);
    server.wait_for_connection();

    return 0;
}
