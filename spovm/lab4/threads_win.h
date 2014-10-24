#ifndef THREADS_WIN_H
#define THREADS_WIN_H

#include <Windows.h>

typedef HANDLE thread_t;
typedef CRITICAL_SECTION mutex_t;
typedef DWORD WINAPI return_value_t;
typedef PVOID input_args_type_t;

struct thread_args_t {
    int id;
    int is_exit[1];
};

return_value_t output_thread(void *input_args);
thread_t create_output_thread(struct thread_args_t *args);
void kill_thread(thread_t thread);
mutex_t mutex_init();
void mutex_lock(mutex_t *mutex);
void mutex_unlock(mutex_t *mutex);
void thread_exit();

#endif // THREADS_WIN_H
