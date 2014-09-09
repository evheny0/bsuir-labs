// #include <sys/types.h>
// #include <stdio.h>
// #include <ncurses.h>

// #define BUF_SIZE 40

// int main(int argc, char const *argv[])
// {
//     pid_t pid = 0;
//     int i;
//     char buf[BUF_SIZE];

//     pid = fork();
//     if (!pid) {
//         sprintf(buf, "This is process %d", pid);
//         puts(buf);
//     } else {
//         waitpid(pid);
//     }
//     return 0;
// }

#include <sys/types.h>
#include <stdio.h>
#include <ncurses.h>

#define BUF_SIZE 40

int main(int argc, char const *argv[])
{
    pid_t pid = 0;
    int i;
    pid = fork();
    if (!pid) {
        execl("/home/eugene0/code/bsuir/spovm/lab1/child", "/home/eugene0/code/bsuir/spovm/lab1/child", "lalal0", NULL);
    }
    waitpid(pid);
    printf("PID: %d\n", getpid());
    printf("child PID: %d\n", pid);
    return 0;
}
