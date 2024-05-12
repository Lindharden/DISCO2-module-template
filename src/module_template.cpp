#include "module.h"
#include "util.h"
#include <opencv2/opencv.hpp>

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

        cv::Mat rawImage(height, width, CV_16UC1, input_image_data);
        rawImage *= 16; // scale image to use 16 bits
        cv::Mat demosaicedImage;
        cv::cvtColor(rawImage, demosaicedImage, cv::COLOR_BayerRG2BGR);
        
        /* Create image metadata before appending */
        Metadata new_meta = METADATA__INIT;
        new_meta.size = demosaicedImage.size().height*demosaicedImage.size().width*3;
        new_meta.width = demosaicedImage.size().width;
        new_meta.height = demosaicedImage.size().height;
        new_meta.channels = 3;
        new_meta.timestamp = timestamp;
        new_meta.bits_pixel = 8;
        new_meta.camera = camera;

        /* Append the image to the result batch */
        append_result_image(demosaicedImage.data, new_meta.size, &new_meta);

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