#ifndef FILE_IO_NIX_H
#define FILE_IO_NIX_H

#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define IN_PROGRESS EINPROGRESS
typedef struct aiocb faio_t;


faio_t faio_init(int file, char *buffer, int file_size);
void faio_start_read(faio_t *control_block);
void faio_start_write(faio_t *control_block);
int faio_get_status(faio_t *control_block);
int faio_wait_end(faio_t *control_block);
int faio_return(faio_t *control_block);

#endif // FILE_IO_NIX_H
