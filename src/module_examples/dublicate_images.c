#include "module.h"
#include "util.h"
/* START MODULE IMPLEMENTATION */
void module()
{

    /* Get metadata of input batch */
    int width = get_input_width();
    int height = get_input_height();
    int channels = get_input_channels();
    int num_images = get_input_num_images();

    int dubs_param = get_param_int("dubs");

    /* Set dimensions of output */
    set_result_dimensions(width, height, channels);
    set_result_num_images(num_images * dubs_param);

    /* Example code for iterating a pixel value at a time */
    for (uint32_t i = 0; i < num_images; ++i)
    {
        unsigned char *input_image_data;
        uint32_t image_size = get_image_data(i, &input_image_data);

        for (uint32_t j = 0; j < dubs_param; j++)
        {
            /* Append the image to the result batch */
            append_result_image(input_image_data, image_size);
        }

        /* Remember to free any allocated memory */
        free(input_image_data);
    }
}

ImageBatch run(ImageBatch *input_batch, ModuleParameterList *module_parameter_list, int *ipc_error_pipe)
{
    ImageBatch result_batch;
    result = &result_batch;
    result->batch_size = 0;
    input = input_batch;
    config = module_parameter_list;
    error_pipe = ipc_error_pipe;

    module();

    finalize();

    return result_batch;
}