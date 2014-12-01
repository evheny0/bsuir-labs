#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include "threads_nix.h"
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <aio.h>
typedef struct aiocb faio_t;
#elif _WIN32
#include "file_io_win.h"
#include "threads_win.h"
#endif

#define FILE_SIZE 1000
#define NUM_OF_FILES 3
const char *OUTPUT_FILENAME = "write_file.txt";
mutex_t mutex_one, mutex_two;

faio_t (*faio_init)(int, char *, int);
void (*faio_start_write)(faio_t *);
void (*faio_start_read)(faio_t *);
int (*faio_wait_end)(faio_t *);

return_value_t output_thread(void *input_args);
void read_files(char **filenames, char *text);
void init_faio_functions();

int main(int argc, char *argv[])
{
    char *filenames[FILE_SIZE] = {"input/read_file1.txt", "input/read_file2.txt", "input/read_file3.txt"};
    char files[FILE_SIZE];
    init_faio_functions();
    mutex_one = mutex_init();
    mutex_two = mutex_init();
    thread_t thread = create_output_thread(files);
    read_files(filenames, files);
    pthread_join(thread, NULL);
    return 0;
}


return_value_t output_thread(input_args_type_t input_args)
{
    char *text = input_args;
    int file = open(OUTPUT_FILENAME, O_WRONLY | O_APPEND, 0);
    mutex_lock(&mutex_one);

    for (int i = 0; i < NUM_OF_FILES; ++i) {
        mutex_lock(&mutex_one);

        faio_t control_block = faio_init(file, text, strlen(text));
        faio_start_write(&control_block);
        faio_wait_end(&control_block);

        mutex_unlock(&mutex_two);
    }
    close(file);
    puts(" ---| End of writing process");
    return 0;
}

void read_files(char **filenames, char *text)
{
    for (int i = 0; i < NUM_OF_FILES; ++i) {
        mutex_lock(&mutex_two);

        puts(filenames[i]);
        int file = open(filenames[i], O_RDONLY, 0);
        faio_t control_block = faio_init(file, text, FILE_SIZE);
        faio_start_read(&control_block);
        faio_wait_end(&control_block);

        mutex_unlock(&mutex_one);
        close(file);
    }
    puts(" ---| End of reading process");
}

void init_faio_functions()
{
    void *lib_handler;
    
    lib_handler = dlopen("/home/eugene0/code/bsuir/spovm/lab5/faio.so", RTLD_LAZY);
    if (lib_handler == NULL) {
        fprintf(stderr, "dlopen error: %s", dlerror());
    }
    
    faio_init = dlsym(lib_handler, "faio_init");
    faio_start_write = dlsym(lib_handler, "faio_start_write");
    faio_start_read = dlsym(lib_handler, "faio_start_read");
    faio_wait_end = dlsym(lib_handler, "faio_wait_end");
}
