#ifndef TYPES_H
#define TYPES_H

#include "module_config.pb-c.h"

typedef struct ImageBatch {
    long mtype;
    int height;
    int width;
    int channels;
    int num_images;
    int shm_key;
    int pipeline_id;
    unsigned char *data;
} ImageBatch;

typedef ImageBatch (*ProcessFunction)(ImageBatch *, ModuleConfig *);

#endif // TYPES_H