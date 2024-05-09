#include "module.h"
#include "util.h"

typedef enum FAIL_CASE
{
    ARITHMETIC,
    DEREFERENCE_NULL_POINTER,
    ACCESS_PROTECTED_MEMORY,
    WRITING_READ_ONLY_MEMORY,
    STACK_OVERFLOW,
    MEMCPY_TO_INVALID_DESTINATION,
    INDEX_OUT_OF_BOUNDS,
    ACCESS_NULL_FUNCTION_POINTER,
    EXCEEDING_MEMORY_ALLOCATION,
    DOUBLE_FREE,
    BUFFER_OVERFLOW,
    SHM_INCREASE_OVERFLOW,
} FAIL_CASE;

typedef void (*FuncPtr)();
void recursion() { 
    recursion(); 
}

/* START MODULE IMPLEMENTATION */
void module()
{
    int fail_case = get_param_int("fail_case");

    switch (fail_case)
    {
    case ARITHMETIC:
        printf("Dividing by zero...\n");
        int x = 1;
        int y = 0;
        int z = x / y;
        break;
    case DEREFERENCE_NULL_POINTER:
        printf("Dereferencing null pointer...\n");
        int *null_ptr = NULL;
        *null_ptr = 1;
        break;
    case ACCESS_PROTECTED_MEMORY:
        printf("Accessing protected memory...\n");
        char *char_ptr = (char*)0x0;
        *char_ptr = 'a';
        break;
    case WRITING_READ_ONLY_MEMORY:
        printf("Writing to read-only memory...\n");
        char *str = "Hello";    
        str[0] = 'h';
        break;
    case STACK_OVERFLOW:
        printf("Initiating infinite recursion...\n");
        recursion();
        break;
    case MEMCPY_TO_INVALID_DESTINATION:
        printf("Copying to invalid destination...\n");
        char *dest = NULL;
        char *src = "Hello";
        memcpy(dest, src, strlen(src));
        break;
    case INDEX_OUT_OF_BOUNDS:
        printf("Indexing out of bounds...\n");
        int arr[5];
        arr[10] = 5;
        break;
    case ACCESS_NULL_FUNCTION_POINTER:
        printf("Calling null function pointer...\n");
        FuncPtr funcPtr = NULL;
        funcPtr();
        break;
    case EXCEEDING_MEMORY_ALLOCATION:
        printf("Allocating 1TB of memory...\n");
        size_t tb = 1000 * 1000 * 1000 * 1000;
        malloc(tb);
        break;
    case DOUBLE_FREE:
        printf("Freeing memory twice...\n");
        int *ptr = (int *)malloc(sizeof(int));
        free(ptr);
        free(ptr);
        break;
    case BUFFER_OVERFLOW:
        printf("Overflowing buffer...\n");
        char buffer[10];
        strcpy(buffer, "This string is more than 10 characters.");
        break;
    case SHM_INCREASE_OVERFLOW:
        Metadata *input_meta = get_metadata(0);
        Metadata new_meta = METADATA__INIT;
        new_meta.size = input_meta->size;
        new_meta.width = input_meta->width;
        new_meta.height = input_meta->height;
        new_meta.channels = input_meta->channels;
        new_meta.timestamp = input_meta->timestamp;
        new_meta.bits_pixel = input_meta->bits_pixel;
        new_meta.camera = input_meta->camera;

        unsigned char * image_data;
        uint32_t image_size = get_image_data(0, &image_data);
        printf("Increased image batch to size: %d...\n", input->batch_size + image_size);
        for (int i = 0; i < get_input_num_images() + 1; i++)
        {
            append_result_image(image_data, image_size, &new_meta);
        }
        free(image_data);   
        break;     
    default:
        printf("'fail_case' not recognized\n");
        break;
    }
}
/* END MODULE IMPLEMENTATION */

/* Main function of module (NO NEED TO MODIFY) */
ImageBatch run(ImageBatch *input_batch, ModuleParameterList *module_parameter_list, int *ipc_error_pipe)
{
    ImageBatch result_batch;
    result = &result_batch;
    result->batch_size = 0;
    input = input_batch;
    config = module_parameter_list;
    error_pipe = ipc_error_pipe;
    initialize();
    
    module();

    finalize();

    return result_batch;
}