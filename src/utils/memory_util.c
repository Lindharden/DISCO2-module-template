#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include "util.h"

void finalize() {
    if (SHARED_MEMORY == 0) return;

    struct shmid_ds info;
    if (shmctl(input->shmid, IPC_STAT, &info) == -1) {
        signal_error_and_exit(304);
    }
    
    size_t shm_size = info.shm_segsz;
    
    if (result->batch_size > shm_size) {
        // Resize is needed: Utilize new unique shared memory ID for storing the batch

        // Detach and free old shared memory segment
        if (shmdt(input->data) == -1) {
            signal_error_and_exit(301);
        }
        if (shmctl(input->shmid, IPC_RMID, NULL) == -1) {
            signal_error_and_exit(302);
        }
        
        int new_shmid = -1;
        struct timespec time;
        // Continously try keys for new shared memory segments
        while (new_shmid == -1) {
            if (clock_gettime(CLOCK_MONOTONIC, &time) < 0)
                signal_error_and_exit(517);

            if ((new_shmid = shmget(time.tv_nsec, result->batch_size, IPC_CREAT | IPC_EXCL | 0666)) != -1) 
                break;

            if (errno == EEXIST) 
                continue;

            signal_error_and_exit(300);
        }
        result->shmid = new_shmid;

        void *shmaddr = shmat(new_shmid, NULL, 0);
        if (shmaddr == NULL) {
            signal_error_and_exit(303);
        }

        memcpy(shmaddr, result->data, result->batch_size);
        free(result->data);
        
        if (shmdt(shmaddr) == -1) {
            signal_error_and_exit(301);
        }
    } else {
        // No resize is needed: We can utilize the old shared memory space
        memcpy(input->data, result->data, result->batch_size); // copy new data to shared memory space of old data
        free(result->data);
        result->shmid = input->shmid; // copy the shared memory key, as we are reusing the space
        if (shmdt(input->data) == -1) {
            signal_error_and_exit(301);
        }
    }
}
