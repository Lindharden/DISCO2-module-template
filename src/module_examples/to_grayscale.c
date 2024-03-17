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

    /* Set dimensions of output */
    set_result_dimensions(width, height, 1);
    set_result_num_images(num_images);

    /* Example code for iterating a pixel value at a time */
    for (int i = 0; i < num_images; ++i)
    {
        unsigned char *input_image_data;
        size_t input_image_size = get_image_data(i, &input_image_data);
        size_t output_image_size = width * height;

        /* Define temporary output image */
        unsigned char *output_image_data = (unsigned char *)malloc(output_image_size);

        if (output_image_data == NULL)
        {
            fprintf(stderr, "[module_template] Error: Unable to allocate memory.\n");
            exit(EXIT_FAILURE);
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

        /* Append the image to the result batch */
        append_result_image(output_image_data, output_image_size);

        /* Remember to free any allocated memory */
        free(input_image_data);
        free(output_image_data);
    }
}
/* END MODULE IMPLEMENTATION */

/* Main function of module (NO NEED TO MODIFY) */
ImageBatch run(ImageBatch *input_batch, ModuleParameterList *module_parameter_list)
{
    ImageBatch result_batch;
    result = &result_batch;
    input = input_batch;
    config = module_parameter_list;

    module();

    finalize();

    return result_batch;
}