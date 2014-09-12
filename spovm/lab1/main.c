#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <ncurses.h>
#include <sys/wait.h>
#elif _WIN32
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#endif

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


char *cut_empty_line(char *str)
{
    str[strlen(str) - 1] = 0;
    return str;
}

#ifdef __linux__
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
#elif _WIN32
void create_child_process(PROCESS_INFORMATION *pi, char *string)
{
    STARTUPINFO si;

    char path[100];
    sprintf(path, "C:\\Users\\homepc\\Desktop\\child.exe %s", string);

    if(!CreateProcess(NULL,   // No module name (use command line)
        "C:\\Users\\homepc\\Desktop\\child.exe 123123123",        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        pi)) {          // Pointer to PROCESS_INFORMATION structure

        printf("CreateProcess failed (%d).\n", GetLastError());
        getchar();
        exit(1);
    }
}

int wait_child_process(PROCESS_INFORMATION *pi)
{
    WaitForSingleObject(pi->hProcess, NULL);
    puts("Success!\n");
    getchar();
}
#endif


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
    PROCESS_INFORMATION pi;
    create_child_process(&pi, buffer);
    wait_child_process(&pi);
#endif

    return 0;
}
