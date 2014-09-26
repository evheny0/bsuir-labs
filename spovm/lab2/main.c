#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX_PIDS = 100;
const char str[] = "Current PID is: ";

void for_child_process(int semaphore_id)
{
    struct sembuf dec, inc, wait;

    dec.sem_num = inc.sem_num = wait.sem_num = 0;
    dec.sem_op = -1;
    inc.sem_op = 1;
    wait.sem_op = 0;

    while (1) {
        semop(semaphore_id, &wait, 1);
        semop(semaphore_id, &inc, 1);
        for (int i = 0; i < strlen(str); i++) {
            printf("%c", str[i]);
        }
        printf("%d\n", getpid());
        semop(semaphore_id, &dec, 1);
    }
}

int create_process(int semaphore_id)
{
    pid_t pid = 0;
    pid = fork();
    if (!pid) {
        for_child_process(semaphore_id);
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

void main_loop(int semaphore_id)
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
    int semaphore_id = create_semaphore(argv[0]);
    main_loop(semaphore_id);

    return 0;
}
