#ifndef TYPES_H
#define TYPES_H
#include <stddef.h>

typedef struct ImageBatch
{
    long mtype;
    int height;
    int width;
    int channels;
    int num_images;
    int batch_size;
    int shm_key;
    int pipeline_id;
    unsigned char *data;
} ImageBatch;

typedef enum ModuleParameter__ValueCase
{
    NOT_SET = 0,
    BOOL_VALUE = 2,
    INT_VALUE = 3,
    FLOAT_VALUE = 4,
    STRING_VALUE = 5
} ModuleParameter__ValueCase;

typedef struct ModuleParameter
{
    char *key;
    ModuleParameter__ValueCase value_case;
    union {
        int bool_value;
        int int_value;
        float float_value;
        char *string_value;
    };
} ModuleParameter;

typedef struct ModuleParameterList
{
    size_t n_parameters;
    ModuleParameter **parameters;
} ModuleParameterList;

#endif // TYPES_H