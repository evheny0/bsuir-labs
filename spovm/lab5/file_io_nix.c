#include "file_io_nix.h"

faio_t faio_init(int file, char *buffer, int file_size)
{
    faio_t control_block;
    memset(&control_block, 0, sizeof(struct aiocb));
    control_block.aio_nbytes = file_size;
    control_block.aio_fildes = file;
    control_block.aio_offset = 0;
    control_block.aio_buf = buffer;
    return control_block;
}

void faio_start_read(faio_t *control_block)
{
    if (aio_read(control_block) == -1) {
        puts(" -- Read error");
    }
    puts(" -- Read process started");
}

void faio_start_write(faio_t *control_block)
{
    if (aio_write(control_block) == -1) {
        puts(" -- Write error");
    }
    puts(" -- Write process started");
}

int faio_get_status(faio_t *control_block)
{
    return aio_error(control_block);
}

int faio_wait_end(faio_t *control_block)
{
    while (faio_get_status(control_block) == IN_PROGRESS) {
        // puts(" -- Reading..");
    }
    puts(" -- End of reading");
    return faio_return(control_block); 
}

int faio_return(faio_t *control_block)
{
    return aio_return(control_block);
}
