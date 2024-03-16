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
    set_result_dimensions(width, height, channels);
    set_result_num_images(num_images);

    /* Retrieve module parameters by name (defined in config.yaml) */
    int param_1 = get_param_bool("param_name_1");
    int param_2 = get_param_int("param_name_2");
    float param_3 = get_param_float("param_name_3");
    char *param_4 = get_param_string("param_name_4");

    /* Example code for iterating a pixel value at a time */
    for (int i = 0; i < num_images; ++i)
    {
        unsigned char *input_image_data;
        size_t image_size = get_image_data(i, &input_image_data);

        /* Define temporary output image */
        unsigned char *output_image_data = (unsigned char *)malloc(image_size);

        if (output_image_data == NULL)
        {
            fprintf(stderr, "[module_template] Error: Unable to allocate memory.\n");
            exit(EXIT_FAILURE);
        }

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                for (int c = 0; c < channels; ++c)
                {
                    /* Accessing pixel data for image i, at position (x, y) and channel c */
                    int pixel_index = y * (width * channels) +
                                      x * channels + c;
                    unsigned char pixel_value = input_image_data[pixel_index];

                    /* Perform any processing here */
                    /* Example: You can manipulate pixel_value or perform any operation */

                    /* If you want to write back processed value, you can do something like this: */
                    output_image_data[pixel_index] = pixel_value;
                }
            }
        }

        /* Append the image to the result batch */
        append_result_image(output_image_data, image_size);

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