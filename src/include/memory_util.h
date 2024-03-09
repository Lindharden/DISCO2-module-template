#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include "types.h"

/**
 * Finalize the resulting image batch data.
 * Copies to new shared memory segment if needed.
 *
 * @param input_batch Pointer to the old image batch.
 * @param result_batch Pointer to the resulting image batch.
 */
void finalize(ImageBatch *input_batch, ImageBatch *result_batch) {
    int shmid = shmget(input_batch->shm_key, 0, 0);
    if (shmid == -1) {
        fprintf(stderr, "Could not get shared memory ID");
        exit(EXIT_FAILURE);
    }
    
    struct shmid_ds info;
    if (shmctl(shmid, IPC_STAT, &info) == -1) {
        fprintf(stderr, "Could not get shared memory stats");
        exit(EXIT_FAILURE);
    }
    
    size_t shm_size = info.shm_segsz;
    size_t result_batch_size = result_batch->height * result_batch->width * result_batch->channels * result_batch->num_images;
    
    if (result_batch_size > shm_size) {
        // Resize is needed: Utilize FTOK to get new unique shared memory ID for storing the batch
        
        int new_shmid = -1;
        key_t new_key;
        while (new_shmid == -1) {
            // Continously generate random key, until we find an unoccupied shared memory address
            new_key = lcg_rand(shmid++);
            new_shmid = shmget(new_key, result_batch_size, IPC_CREAT | IPC_EXCL | 0666);
        }

        void *shmaddr = shmat(new_shmid, NULL, 0);
        if (shmaddr == (void*)-1) {
            exit(EXIT_FAILURE);
        }

        memcpy(shmaddr, result_batch->data, result_batch_size);
        result_batch->shm_key = new_key;

        // Detach and free old shared memory segment
        shmdt(input_batch->data);
        shmctl(shmid, IPC_RMID, NULL);
    } else {
        // No resize is needed: We can utilize the old shared memory space
        memcpy(input_batch->data, result_batch->data, result_batch_size);
        result_batch->shm_key = input_batch->shm_key;
    }

    free(result_batch->data);
}


#include <stdint.h>

// Linear Congruential Generator
// Constants are from Numerical Recipes
// The modulo is implicit in the 32-bit unsigned integer's wraparound.
uint32_t lcg_rand(uint32_t *seed) {
    const uint32_t a = 1664525;
    const uint32_t c = 1013904223;
    *seed = a * (*seed) + c;
    return *seed;
}

// Initialize the seed
uint32_t seed = 1234; // Example seed value

// Example usage
#include <stdio.h>

int main() {

}