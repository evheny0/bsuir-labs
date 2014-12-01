#ifndef THREADS_NIX_H
#define THREADS_NIX_H

#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

typedef pthread_t thread_t;
typedef pthread_mutex_t mutex_t;
typedef void * return_value_t;
typedef void * input_args_type_t;


return_value_t output_thread(void *input_args);
thread_t create_output_thread(void *args);
void kill_thread(thread_t thread);
mutex_t mutex_init();
void mutex_lock(mutex_t *mutex);
void mutex_unlock(mutex_t *mutex);
void thread_exit();

#endif // THREADS_NIX_H
