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

    /* Retrieve module parameters by name (defined in config.yaml) */
    int param_1 = get_param_bool("param_name_1");
    int param_2 = get_param_int("param_name_2");
    float param_3 = get_param_float("param_name_3");
    char *param_4 = get_param_string("param_name_4");

    /* Example code for iterating a pixel value at a time */
    for (int i = 0; i < num_images; ++i)
    {
        Metadata *input_meta = get_metadata(i);
        int height = input_meta->height;
        int width = input_meta->width;
        int channels = input_meta->channels;
        int timestamp = input_meta->timestamp;
        int bits_pixel = input_meta->bits_pixel;
        char *camera = input_meta->camera;

        /* Get custom metadata values */
        // int example_bool = get_custom_metadata_bool(input_meta, "example_bool");
        // int int_example = get_custom_metadata_int(input_meta, "example_int");
        // float example_float = get_custom_metadata_float(input_meta, "example_float");
        // char *example_string = get_custom_metadata_string(input_meta, "example_string");
        
        unsigned char *input_image_data;
        size_t size = get_image_data(i, &input_image_data);

        /* Define temporary output image */
        unsigned char *output_image_data = (unsigned char *)malloc(size);

        /* Check for malloc error */
        if (output_image_data == NULL)
        {
            signal_error_and_exit(MALLOC_ERR);
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
        
        /* Create image metadata before appending */
        Metadata new_meta = METADATA__INIT;
        new_meta.size = size;
        new_meta.width = width;
        new_meta.height = height;
        new_meta.channels = channels;
        new_meta.timestamp = timestamp;
        new_meta.bits_pixel = bits_pixel;
        new_meta.camera = camera;

        /* Add custom metadata key-value */
        add_custom_metadata_bool(&new_meta, "example_bool", true);
        add_custom_metadata_int(&new_meta, "example_int", 20);
        add_custom_metadata_float(&new_meta, "example_float", 20.5);
        add_custom_metadata_string(&new_meta, "example_string", "TEST");

        /* Append the image to the result batch */
        append_result_image(output_image_data, size, &new_meta);

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
    input = input_batch;
    config = module_parameter_list;
    error_pipe = ipc_error_pipe;
    initialize();

    module();

    finalize();

    return result_batch;
}