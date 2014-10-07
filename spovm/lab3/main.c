#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int DEC = 0, INC = 1, WAIT = 2;
const int STRING_SIZE = 100;
int file_descriptors[2];

typedef int sem_t;


struct sembuf *create_sembufs()
{
    struct sembuf *result = calloc(3, sizeof(struct sembuf));
    result[DEC].sem_num = result[INC].sem_num = result[WAIT].sem_num = 0;
    result[DEC].sem_op = -1;
    result[INC].sem_op = 1;
    result[WAIT].sem_op = 0;
    return result;
}

void child_program(sem_t semaphore_id)
{
    close(file_descriptors[1]);

    char recieved_string[80];
    struct sembuf *sembufs = create_sembufs();

    while (recieved_string[0] != 'q') {
        semop(semaphore_id, &sembufs[INC], 1);
        semop(semaphore_id, &sembufs[WAIT], 1);
        printf("Received!\n");
        read(file_descriptors[0], recieved_string, sizeof(recieved_string));
        printf("Received string is: %s", recieved_string);
        semop(semaphore_id, &sembufs[DEC], 1);
    }
    free(sembufs);
    exit(0);
}

int create_process(sem_t semaphore_id)
{
    pid_t pid = 0;
    pid = fork();
    if (!pid) {
        child_program(semaphore_id);
    }
    return pid;
}

void kill_process(int pid)
{
    kill(pid, SIGABRT);
}

void main_loop(sem_t semaphore_id)
{
    char string[STRING_SIZE];
    struct sembuf *sembufs = create_sembufs();
    int child_pid = create_process(semaphore_id);
    close(file_descriptors[0]);

    while (string[0] != 'q') {
        printf("Enter string: ");
        fgets(string, STRING_SIZE, stdin);
        write(file_descriptors[1], string, (strlen(string)+1));
        semop(semaphore_id, &sembufs[DEC], 1);
        semop(semaphore_id, &sembufs[INC], 1);
        semop(semaphore_id, &sembufs[WAIT], 1);
    }
    free(sembufs);
}

int create_semaphore(const char* path)
{
    int id = semget(123123, 1, IPC_CREAT | 0600);
    semctl(id, 0, SETVAL, 0);
    return id;
}

void init_pipe(int *file_descriptors)
{
    pipe(file_descriptors);
}

int main(int argc, char const *argv[])
{
    sem_t semaphore_id = create_semaphore(argv[0]);
    init_pipe(file_descriptors);
    main_loop(semaphore_id);

    return 0;
}
