#include "threads_win.h"

thread_t create_output_thread(struct thread_args_t *args)
{
    SECURITY_ATTRIBUTES attr;
    HANDLE handle;

    attr.bInheritHandle = TRUE;
    attr.nLength = sizeof(attr);
    attr.lpSecurityDescriptor = NULL;
    return CreateThread(&attr, 0, output_thread, args, 0, NULL);
}

mutex_t mutex_init()
{
    CRITICAL_SECTION mutex;
    InitializeCriticalSection(&mutex);
    return mutex;
}

void mutex_lock(mutex_t *mutex)
{
    EnterCriticalSection(mutex);
}

void mutex_unlock(mutex_t *mutex)
{
    LeaveCriticalSection(mutex);
}

void thread_exit()
{
    ExitThread(0);
}
