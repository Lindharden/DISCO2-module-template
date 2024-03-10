#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include "util.h"

void finalize(ImageBatch *input_batch, ImageBatch *result_batch) {
    if (SHARED_MEMORY == 0) return;

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
    
    size_t result_batch_size = result_batch->height * result_batch->width * result_batch->channels * result_batch->num_images;
    size_t shm_size = info.shm_segsz;
    
    if (result_batch_size > shm_size) {
        // Resize is needed: Utilize new unique shared memory ID for storing the batch        
        int new_shmid = -1;
        key_t new_key = input_batch->shm_key;
        while (new_shmid == -1) {
            // Continously try keys for new shared memory segments
            new_shmid = shmget(++new_key, result_batch_size, IPC_CREAT | IPC_EXCL | 0666);
        }
        result_batch->shm_key = new_key;

        void *shmaddr = shmat(new_shmid, NULL, 0);
        if (shmaddr == (void*)-1) {
            exit(EXIT_FAILURE);
        }

        memcpy(shmaddr, result_batch->data, result_batch_size);
        result_batch->data = shmaddr;

        // Detach and free old shared memory segment
        if (shmdt(input_batch->data) == -1) {
            fprintf(stderr, "Error detaching old shared memory: %s\n");
            exit(EXIT_FAILURE);
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            fprintf(stderr, "Error removing old shared memory: %s\n");
            exit(EXIT_FAILURE);
        }
    } else {
        // No resize is needed: We can utilize the old shared memory space
        memcpy(input_batch->data, result_batch->data, result_batch_size); // copy new data to shared memory space of old data
        result_batch->data = input_batch->data; // set new image batch to point to the shared memory space of the old data
        result_batch->shm_key = input_batch->shm_key; // copy the shared memory key, as we are reusing the space
    }
}
