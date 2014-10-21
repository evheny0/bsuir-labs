#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int STRING_SIZE = 100;

#ifdef __linux__
const int DEC = 0, INC = 1, WAIT = 2;
int file_descriptors[2];
typedef int sem_t;

#elif WIN_32
const int PIPE_PATH = "\\\\.\\pipe\\lab3";
const int PATH = "C:\\Users\\homepc\\Desktop\\lab3\\lab3\\bin\\Debug\\lab3.exe 123123123";
typedef HANDLE sem_t;

#endif

#ifdef __linux__
struct sembuf *create_sembufs()
{
    struct sembuf *result = calloc(3, sizeof(struct sembuf));
    result[DEC].sem_num = result[INC].sem_num = result[WAIT].sem_num = 0;
    result[DEC].sem_op = -1;
    result[INC].sem_op = 1;
    result[WAIT].sem_op = 0;
    return result;
}
#elif WIN_32
HANDLE open_semaphore()
{
    return OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "sem");
}

void wait_semaphore(HANDLE id)
{
    WaitForSingleObject(id, INFINITE);
}

void release_semaphore(HANDLE id)
{
    ReleaseSemaphore(id, 1, NULL);
}

HANDLE create_pipe()
{
    HANDLE pipe = CreateNamedPipe(PIPE_PATH,    // Name
                                  PIPE_ACCESS_DUPLEX, // OpenMode
                                  PIPE_TYPE_MESSAGE | PIPE_READMODE_BYTE | PIPE_NOWAIT, // PipeMode
                                  3,            // MaxInstances
                                  1024,         // OutBufferSize
                                  1024,         // InBuffersize
                                  1,            // TimeOut
                                  NULL);        // Security
    if (pipe == INVALID_HANDLE_VALUE)
    {
        printf("Could not create the pipe\n");
        exit(1);
    }
    ConnectNamedPipe(pipe, NULL);
    return pipe;
}
#endif

#ifdef __linux__
void child_process(sem_t semaphore_id)
{
    close(file_descriptors[1]);

    char recieved_string[80];
    struct sembuf *sembufs = create_sembufs();

    while (recieved_string[0] != 'q') {
        semop(semaphore_id, &sembufs[INC], 1);
        semop(semaphore_id, &sembufs[WAIT], 1);
        printf("  * Received!\n");
        read(file_descriptors[0], recieved_string, sizeof(recieved_string));
        printf("  * Received string is: %s", recieved_string);
        printf("  * Return control to parent\n");
        semop(semaphore_id, &sembufs[DEC], 1);
    }
    free(sembufs);
    exit(0);
}
#elif WIN_32
void child_process()
{
    HANDLE semaphore = create_semaphore();
    HANDLE pipe = CreateFile(PIPE_PATH, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL  , NULL);
    char string[100];
    int i = 0;
    DWORD dwBytesRead;
    HANDLE semaphore = open_semaphore();

    do {
        wait_semaphore(semaphore);
        puts("  * Recieved!");
        for (i = 0; ReadFile(pipe, string + i, 1, &dwBytesRead, NULL); i++) {
            if (!string[i]) {
                break;
            }
        }
        string[i] = '\0';
        puts(string);
        puts("  * Return control to parent");
        release_semaphore(semaphore);
    } while (string[0] != 'q');

    CloseHandle(pipe);
}
#endif


#ifdef __linux__
int create_process(sem_t semaphore_id)
{
    pid_t pid = 0;
    pid = fork();
    if (!pid) {
        child_process(semaphore_id);
    }
    return pid;
}
#elif WIN_32
HANDLE init_child(sem_t semaphore_id)
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(NULL, PATH, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        getchar();
        exit(1);
    }
    return pi.hProcess;
}
void check_if_child_process(int argc, char const *argv[])
{
    if (argc > 1) {
        child_process();
        exit(0);
    } else {
        init_child();
    }
}

#endif

void kill_process(int pid)
{
    kill(pid, SIGABRT);
}

void main_loop(sem_t semaphore_id)
{
    char string[STRING_SIZE];

    #ifdef __linux__
    struct sembuf *sembufs = create_sembufs();
    create_process(semaphore_id);
    close(file_descriptors[0]);
    #elif WIN_32
    HANDLE pipe = create_pipe();
    DWORD size;

    #endif

    do {
        puts("  -- Enter string: ");
        fgets(string, STRING_SIZE, stdin);

        #ifdef __linux__
        write(file_descriptors[1], string, (strlen(string)+1));
        puts("  -- Sent string");
        semop(semaphore_id, &sembufs[DEC], 1);
        semop(semaphore_id, &sembufs[INC], 1);
        semop(semaphore_id, &sembufs[WAIT], 1);

        #elif WIN_32
        WriteFile(pipe, string, strlen(string) + 1, &size, NULL);
        puts("  -- Sent string");
        release_semaphore(semaphore_id);
        wait_semaphore(semaphore_id);
        #endif

    } while (string[0] != 'q');

}

#ifdef __linux__
sem_t create_semaphore(const char* path)
{
    sem_t id = semget(ftok(path, 'i'), 1, IPC_CREAT | 0600);
    semctl(id, 0, SETVAL, 0);
    return id;
}
#elif WIN_32
sem_t create_semaphore(const char* path)
{
    SECURITY_ATTRIBUTES attr;
    sem_t handle;
    attr.bInheritHandle = TRUE;
    attr.nLength = sizeof(attr);
    attr.lpSecurityDescriptor = NULL;
    handle = CreateSemaphore(&attr, 0, 1, "sem");
    return handle;
}
#endif

void init_pipe(int *file_descriptors)
{
    pipe(file_descriptors);
}

int main(int argc, char const *argv[])
{
    sem_t semaphore_id = create_semaphore(argv[0]);

    #ifdef __linux__
    init_pipe(file_descriptors);
    #elif WIN_32
    check_if_child_process(argc, argv);
    #endif

    main_loop(semaphore_id);

    return 0;
}
