#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


const char output_string[] = "Current PID is: ";

typedef int sem_t;



void check_arguments(int argc)
{
    if (argc < 1) {
        exit(1);
    }
}

sem_t get_sem_from_argv(char const *argv[])
{
    return atoi(argv[1]);
}

void print_output_string()
{
    for (int i = 0; i < strlen(output_string); i++) {
        printf("%c", output_string[i]);
    }
}

int main(int argc, char const *argv[])
{
    check_arguments(argc);
    sem_t semaphore_id = get_sem_from_argv(argv);

    struct sembuf dec, inc, wait;
    dec.sem_num = inc.sem_num = wait.sem_num = 0;
    dec.sem_op = -1;
    inc.sem_op = 1;
    wait.sem_op = 0;

    while (1) {
        semop(semaphore_id, &wait, 1);
        semop(semaphore_id, &inc, 1);
        print_output_string();
        printf("%d\n", getpid());
        semop(semaphore_id, &dec, 1);
    }

    return 0;
}
