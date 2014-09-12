#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/wait.h>

const char *PATH_TO_CHILD = "child";
const int MAX_STRING_SIZE = 100;


void check_status(int status)
{
    if (status) {
        puts("Some error ocured\n");
    } else {
        puts("Success!\n");
    }
}

void create_child_process(char *string)
{
    execl(PATH_TO_CHILD, PATH_TO_CHILD, string, NULL);
    exit(1);  // exec only if some error will ocured
}

int wait_child_process(int pid)
{
    int status;
    waitpid(pid, &status, 0);
    printf(" *** PID: %d\n", getpid());

    check_status(status);
    return status;
}

char *cut_empty_line(char *str)
{
    str[strlen(str) - 1] = 0;
    return str;
}



int main(int argc, char const *argv[])
{
    printf(" *** PID: %d\n", getpid());
    char buffer[MAX_STRING_SIZE];

    puts("Enter string");
    fgets(buffer, MAX_STRING_SIZE, stdin);
    cut_empty_line(buffer);

#ifdef __linux__

    pid_t pid = 0;
    pid = fork();
    if (!pid) {
        create_child_process(buffer);
    }
    wait_child_process(pid);

#elif _WIN32

    puts("PROVAL");

#endif

    return 0;
}
