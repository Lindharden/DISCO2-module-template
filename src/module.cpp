#include "module.h"
#include "util.h"

/* Main function of module */
ImageBatch run(ImageBatch *input_batch, ModuleParameterList *config) {

    /* Create a result batch, replace with expected dimensions */
    ImageBatch result_batch;
    result_batch.width = input_batch->width;
    result_batch.height = input_batch->height;
    result_batch.channels = input_batch->channels;
    result_batch.num_images = input_batch->num_images;

    /* Retrieve module parameters by name (defined in config.yaml) */
    int param_1 = get_param_bool(config, "param_name_1");
    int param_2 = get_param_int(config, "param_name_2");
    float param_3 = get_param_float(config, "param_name_3");
    char *param_4 = get_param_string(config, "param_name_4");

    /* Allocate memory for resulting image data, fit to expected output size */
    int data_size = result_batch.width * result_batch.height * result_batch.channels * result_batch.num_images;
    unsigned char *new_data = (unsigned char *)malloc(data_size);
    result_batch.data = new_data;

    /* Example code for iterating a pixel value at a time */
    for (int i = 0; i < input_batch->num_images; ++i) {
        for (int y = 0; y < input_batch->height; ++y) {
            for (int x = 0; x < input_batch->width; ++x) {
                for (int c = 0; c < input_batch->channels; ++c) {
                    /* Accessing pixel data for image i, at position (x, y) and channel c */
                    int pixel_index = i * (input_batch->width * input_batch->height * input_batch->channels) +
                                    y * (input_batch->width * input_batch->channels) +
                                    x * input_batch->channels + c;
                    unsigned char pixel_value = input_batch->data[pixel_index];

                    /* Perform any processing here */
                    /* Example: You can manipulate pixel_value or perform any operation */

                    /* If you want to write back processed value, you can do something like this: */
                    result_batch.data[pixel_index] = pixel_value;
                }
            }
        }
    }
    
    /* Finalize the module job (MUST BE HERE!) */
    finalize(input_batch, &result_batch);

    return result_batch;
}