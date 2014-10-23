#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include "threads_nix.h"
#elif _WIN32
#include "threads_win.h"
#endif

const int STRING_SIZE = 20;
const thread_t MAX_THREADS_COUNT = 100;
const char *OUTPUT_STRING = "Thread id is: ";
mutex_t mutex;

void main_loop();
void *output_thread(void *input_args);
void add_thread(thread_t *threads, struct thread_args_t *all_args, int last);
void delete_thread(thread_t *threads, struct thread_args_t *all_args, int *last);
void kill_all_threads(thread_t *threads, struct thread_args_t *all_args, int last);


int main(int argc, char const *argv[])
{
    mutex = mutex_init();
    main_loop();
    return 0;
}

void main_loop()
{
    char input[STRING_SIZE];
    thread_t threads[MAX_THREADS_COUNT];
    struct thread_args_t all_args[MAX_THREADS_COUNT];
    int last_thread_id = -1;

    while (1) {
        fgets(input, STRING_SIZE, stdin);
        switch (input[0]) {
        case '+':
            add_thread(threads, all_args, ++last_thread_id);
            break;
        case '-':
            delete_thread(threads, all_args, &last_thread_id);
            break;
        case 'q':
            kill_all_threads(threads, all_args, last_thread_id);
            return;
        }
    }
}

void *output_thread(void *input_args)
{
    struct thread_args_t *args = (struct thread_args_t *) input_args;
    int is_exit = 0;

    while (!is_exit) {
        mutex_lock(&mutex);
        for (int i = 0; i < strlen(OUTPUT_STRING); ++i) {
            printf("%c", OUTPUT_STRING[i]);
        }
        printf("%d\n", args->id);
        if (*(args->is_exit)) {
            is_exit = 1;
        }
        mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
    return 0;
}

void add_thread(thread_t *threads, struct thread_args_t *all_args, int last)
{
    all_args[last].id = last;
    *(all_args[last].is_exit) = 0;
    threads[last] = create_output_thread(&(all_args[last]));
}

void delete_thread(thread_t *threads, struct thread_args_t *all_args, int *last)
{
    if (*last >= 0 && *last < MAX_THREADS_COUNT) {
        mutex_lock(&mutex);
        *(all_args[*last].is_exit) = 1;
        mutex_unlock(&mutex);
        (*last)--;
    }
}

void kill_all_threads(thread_t *threads, struct thread_args_t *all_args, int last)
{
    for (int i = 0; i <= last; ++i) {
        mutex_lock(&mutex);
        *(all_args[i].is_exit) = 1;
        mutex_unlock(&mutex);
    }
}
