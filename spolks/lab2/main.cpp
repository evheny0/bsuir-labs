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

    return 0;
}
