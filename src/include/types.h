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
    int shm_key;
    int pipeline_id;
    unsigned char *data;
} ImageBatch;

typedef enum 
{
    CONFIG_PARAMETER__VALUE__NOT_SET = 0,
    CONFIG_PARAMETER__VALUE_BOOL_VALUE = 2,
    CONFIG_PARAMETER__VALUE_INT_VALUE = 3,
    CONFIG_PARAMETER__VALUE_FLOAT_VALUE = 4,
    CONFIG_PARAMETER__VALUE_STRING_VALUE = 5
} ConfigParameter__ValueCase;

typedef struct ConfigParameter
{
    char *key;
    ConfigParameter__ValueCase value_case;
    union {
        int bool_value;
        int int_value;
        float float_value;
        char *string_value;
    };
} ConfigParameter;

typedef struct ModuleConfig
{
    size_t n_parameters;
    ConfigParameter **parameters;
} ModuleConfig;

#endif // TYPES_H