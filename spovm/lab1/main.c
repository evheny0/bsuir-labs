//#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include <sys/wait.h>

const char *PATH_TO_CHILD = "child";
const int MAX_STRING_SIZE = 100;


void create_child_process(char *string)
{
    execl(PATH_TO_CHILD, PATH_TO_CHILD, string, NULL);
}

int wait_child_process(int pid)
{
    int status;
    waitpid(pid, &status, 0);
    printf("*** PID: %d\n", getpid());

    if (status) {
        puts("Some error ocured\n");
    } else {
        puts("Success!\n");
    }
    return status;
}

char *cut_string(char *str)
{
    str[strlen(str) - 1] = 0;
    return str;
}



int main(int argc, char const *argv[])
{
    printf("*** PID: %d\n", getpid());
    char buffer[MAX_STRING_SIZE];
    pid_t pid = 0;

    fgets(buffer, MAX_STRING_SIZE, stdin);
    cut_string(buffer);
    pid = fork();

    if (!pid) {
        create_child_process(buffer);
    }
    wait_child_process(pid);

    return 0;
}
