#include "util.h"
#include "globals.h"
#include <unistd.h>
#include <sys/shm.h>

int *error_pipe;

void signal_error_and_exit(uint16_t error_code)
{
    if (SHARED_MEMORY)
        write(error_pipe[1], &error_code, sizeof(uint16_t));

    if (!SHARED_MEMORY)
        fprintf(stderr, "Error with code %d occurred.\n", error_code);
        // Detach and free shared memory
    shmdt(input->data);
    exit(EXIT_FAILURE);
}