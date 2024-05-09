#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include "util.h"

void finalize() {
    if (SHARED_MEMORY == 0) return;

    int shmid = shmget(input->shm_key, 0, 0);
    if (shmid == -1) {
        signal_error_and_exit(300);
    }
    
    struct shmid_ds info;
    if (shmctl(shmid, IPC_STAT, &info) == -1) {
        signal_error_and_exit(300);
    }
    
    size_t shm_size = info.shm_segsz;
    
    if (result->batch_size > shm_size) {
        // Resize is needed: Utilize new unique shared memory ID for storing the batch        
        int new_shmid = -1;
        key_t new_key = input->shm_key;
        // Continously try keys for new shared memory segments
        while (new_shmid == -1) {
            if ((new_shmid = shmget(++new_key, result->batch_size, IPC_CREAT | IPC_EXCL | 0666)) != -1) 
                break;

            if (errno == EEXIST) 
                continue;

            signal_error_and_exit(300);
        }
        result->shm_key = new_key;

        void *shmaddr = shmat(new_shmid, NULL, 0);
        if (shmaddr == (void*)-1) {
            exit(EXIT_FAILURE);
        }

        memcpy(shmaddr, result->data, result->batch_size);
        free(result->data);
        result->data = shmaddr;

        // Detach and free old shared memory segment
        if (shmdt(input->data) == -1) {
            signal_error_and_exit(301);
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            signal_error_and_exit(302);
        }
    } else {
        // No resize is needed: We can utilize the old shared memory space
        memcpy(input->data, result->data, result->batch_size); // copy new data to shared memory space of old data
        free(result->data);
        result->data = input->data; // set new image batch to point to the shared memory space of the old data
        result->shm_key = input->shm_key; // copy the shared memory key, as we are reusing the space
    }
}
