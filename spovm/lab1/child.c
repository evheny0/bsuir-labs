#include <sys/types.h>
#include <stdio.h>
#include <ncurses.h>

#define BUF_SIZE 40

int main(int argc, char const *argv[])
{
    int i;
    //execl("child", NULL);
    printf("PID child exec: %d\n", getpid());


    return 0;
}
