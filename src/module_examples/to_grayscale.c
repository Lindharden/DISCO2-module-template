#include "module.h"
#include "util.h"

/* Define custom error codes in the range of 1-99*/
enum ERROR_CODE {
    MALLOC_ERR = 1,
    PLACEHOLDER = 2,
};

/* START MODULE IMPLEMENTATION */
void module()
{
    /* Example code for iterating a pixel value at a time */
    for (int i = 0; i < num_images; ++i)
    {
        Metadata *input_meta = get_metadata(i);
        int size = input_meta->size;
        int height = input_meta->height;
        int width = input_meta->width;
        int channels = input_meta->channels;
        int timestamp = input_meta->timestamp;
        int bits_pixel = input_meta->bits_pixel;
        char *camera = input_meta->camera;

        unsigned char *input_image_data;
        get_image_data(i, &input_image_data);
        uint32_t output_image_size = width * height;

        /* Define temporary output image */
        unsigned char *output_image_data = (unsigned char *)malloc(output_image_size);

        if (output_image_data == NULL)
        {
            signal_error_and_exit(MALLOC_ERR);
        }

        for (int i = 0; i < input_image_size; i += channels) 
        {
            // Extract color components from the original pixel
            unsigned char r = input_image_data[i];
            unsigned char g = input_image_data[i + 1];
            unsigned char b = input_image_data[i + 2];

            // Convert to grayscale using luminance method (0.299R + 0.587G + 0.114B)
            unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

            output_image_data[i / channels] = gray;
        }

        Metadata new_meta = METADATA__INIT;
        new_meta.size = size;
        new_meta.width = width;
        new_meta.height = height;
        new_meta.channels = 1;
        new_meta.timestamp = timestamp;
        new_meta.bits_pixel = bits_pixel;
        new_meta.camera = camera;

        /* Append the image to the result batch */
        append_result_image(output_image_data, output_image_size, &new_meta);

        /* Remember to free any allocated memory */
        free(input_image_data);
        free(output_image_data);
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
    unpack_metadata();

    module();

    finalize();

    return result_batch;
}