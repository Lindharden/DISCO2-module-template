#include "module.h"
#include "util.h"

/* Define custom error codes */
enum ERROR_CODE {
    MALLOC_ERR = 1,
    PLACEHOLDER = 2,
};

/* START MODULE IMPLEMENTATION */
void module()
{
    /* Get number of images in input batch */
    int num_images = get_input_num_images();

    /* Example code for iterating a pixel value at a time */
    for (int i = 0; i < num_images; ++i)
    {
        Metadata *input_meta = get_metadata(i);
        
        unsigned char *input_image_data;
        size_t size = get_image_data(i, &input_image_data);

        Metadata new_meta = METADATA__INIT;
        new_meta.size = input_meta->size;
        new_meta.width = input_meta->width;
        new_meta.height = input_meta->height;
        new_meta.channels = input_meta->channels;
        new_meta.timestamp = input_meta->timestamp;
        new_meta.bits_pixel = input_meta->bits_pixel;
        new_meta.camera = input_meta->camera;

        /* Append the image to the result batch */
        append_result_image(input_image_data, size, &new_meta);

        /* Remember to free any allocated memory */
        free(input_image_data);
    }
}
/* END MODULE IMPLEMENTATION */

/* Main function of module (NO NEED TO MODIFY) */
ImageBatch run(ImageBatch *input_batch, ModuleParameterList *module_parameter_list, int *ipc_error_pipe)
{
    ImageBatch result_batch;
    result = &result_batch;
    input = input_batch;
    config = module_parameter_list;
    error_pipe = ipc_error_pipe;
    initialize();

    module();

    finalize();

    return result_batch;
}