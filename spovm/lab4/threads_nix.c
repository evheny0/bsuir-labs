#include "threads_nix.h"

thread_t create_output_thread(struct thread_args_t *args)
{
    pthread_t thread;
    pthread_create(&thread, NULL, output_thread, args);
    return thread;
}

void kill_thread(thread_t thread)
{
    pthread_kill(thread, SIGCONT);
}

mutex_t mutex_init()
{
    mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&mutex, NULL);
    return mutex;
}

void mutex_lock(mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
}

void mutex_unlock(mutex_t *mutex)
{
    pthread_mutex_unlock(mutex);
}

