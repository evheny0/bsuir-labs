#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX_PIDS = 100;
const char *PATH_TO_CHILD = "child";

typedef int sem_t;


char *sem_to_string(sem_t semaphore_id)
{
    char *str = calloc(100, sizeof(char));
    sprintf(str, "%d", (int)semaphore_id);
    return str;
}

int create_process(sem_t semaphore_id)
{
    pid_t pid = 0;
    pid = fork();
    if (!pid) {
        execl(PATH_TO_CHILD, PATH_TO_CHILD, sem_to_string(semaphore_id), NULL);
    }
    return pid;
}

void kill_process(int pid)
{
    kill(pid, SIGABRT);
}

void kill_childs(int *pids, int pid_counter)
{
    while (pid_counter > 0) {
        kill_process(pids[pid_counter]);
        pid_counter--;
    }
}

void main_loop(sem_t semaphore_id)
{
    int end = 0, pid_counter = 0, pids[MAX_PIDS];

    while (!end) {
        switch (getchar()) {
        case '+':
            pids[++pid_counter] = create_process(semaphore_id);
            break;
        case '-':
            kill_process(pids[pid_counter--]);
            break;
        case 'q':
            kill_childs(pids, pid_counter);
            end = 1;
            break;
        }
    }
}

int create_semaphore(const char* path)
{
    int id = semget(ftok(path, 'i'), 1, IPC_CREAT);
    semctl(id, 0, SETVAL, 0);
    return id;
}



int main(int argc, char const *argv[])
{
    sem_t semaphore_id = create_semaphore(argv[0]);
    main_loop(semaphore_id);

    return 0;
}
