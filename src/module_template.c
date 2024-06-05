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

    /* Retrieve module parameters by name if any (defined in config.yaml) */
    // int param_1 = get_param_bool("param_name_1");
    // int param_2 = get_param_int("param_name_2");
    // float param_3 = get_param_float("param_name_3");
    // char *param_4 = get_param_string("param_name_4");

    /* Example code for iterating a pixel value at a time */
    for (int i = 0; i < num_images; ++i)
    {
        Metadata *input_meta = get_metadata(i);

        /* Get custom metadata values */
        // int example_bool = get_custom_metadata_bool(input_meta, "example_bool");
        // int int_example = get_custom_metadata_int(input_meta, "example_int");
        // float example_float = get_custom_metadata_float(input_meta, "example_float");
        // char *example_string = get_custom_metadata_string(input_meta, "example_string");
        
        unsigned char *image_data;
        size_t size = get_image_data(i, &image_data);

        Metadata new_meta = METADATA__INIT;
        new_meta.size = input_meta->size;
        new_meta.width = input_meta->width;
        new_meta.height = input_meta->height;
        new_meta.channels = input_meta->channels;
        new_meta.timestamp = input_meta->timestamp;
        new_meta.bits_pixel = input_meta->bits_pixel;
        new_meta.camera = input_meta->camera;

        /* Add custom metadata key-value, if any new meta data is to be added */
        // add_custom_metadata_bool(&new_meta, "example_bool", 1);
        // add_custom_metadata_int(&new_meta, "example_int", 20);
        // add_custom_metadata_float(&new_meta, "example_float", 20.5);
        // add_custom_metadata_string(&new_meta, "example_string", "TEST");

        /* Append the image to the result batch */
        append_result_image(image_data, size, &new_meta);

        /* Remember to free any allocated memory */
        free(image_data);
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
