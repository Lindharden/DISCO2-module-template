#include "module.h"
#include "util.h"
#include <webp/encode.h>

void module()
{
    set_result_dimensions(get_input_width(), get_input_height(), get_input_channels());
    set_result_num_images(get_input_num_images());

    int stride = get_input_width() * sizeof(uint8_t) * 4; // amount of bytes per row

    /* Perform lossless compression for each image in the batch */
    for (uint32_t img_index = 0; img_index < get_input_num_images(); img_index++)
    {
        // Get input image data
        unsigned char *input_image_data;
        get_image_data(img_index, &input_image_data);

        // Compress the image and create result data
        uint8_t *result_image_data = NULL;
        uint32_t output_size = WebPEncodeLosslessRGBA(input_image_data, get_input_width(), get_input_height(), stride, &result_image_data);

        append_result_image(result_image_data, output_size);

        /* Code for saving webp to file */
        // FILE *outfile = fopen("output.webp", "wb");
        // fwrite(result_image_data, 1, output_size, outfile);
        // fclose(outfile);
    }
}

/* Main function of module */
ImageBatch run(ImageBatch *input_batch, ModuleParameterList *module_parameter_list, int *ipc_error_pipe)
{
    /* Create a result batch, replace with expected dimensions */
    ImageBatch result_batch;
    input = input_batch;
    result->batch_size = 0;
    result = &result_batch;
    config = module_parameter_list;
    error_pipe = ipc_error_pipe;

    module();

    /* Finalize the module job (MUST BE HERE!) */
    finalize(input_batch, &result_batch);

    return result_batch;
}