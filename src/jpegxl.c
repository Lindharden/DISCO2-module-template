#include "module.h"
#include "util.h"
#include <jxl/encode.h>

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
        int size = input_meta->size;
        int height = input_meta->height;
        int width = input_meta->width;
        int channels = input_meta->channels;
        int timestamp = input_meta->timestamp;
        int bits_pixel = input_meta->bits_pixel;
        char *camera = input_meta->camera;

        unsigned char *input_image_data;
        get_image_data(i, &input_image_data);

        JxlEncoder* encoder = JxlEncoderCreate(NULL);    
        JxlEncoderFrameSettings* settings = JxlEncoderFrameSettingsCreate(encoder, NULL);

        JxlBasicInfo basic_info;
        JxlEncoderInitBasicInfo(&basic_info);
        basic_info.xsize = width;
        basic_info.ysize = height;
        basic_info.num_color_channels = channels;
        basic_info.bits_per_sample = bits_pixel;

        JxlPixelFormat format = {channels, JXL_TYPE_UINT8, JXL_NATIVE_ENDIAN, 0};
        JxlEncoderSetBasicInfo(encoder, &basic_info);
        JxlEncoderAddImageFrame(settings, &format, input_image_data, size);

        JxlEncoderCloseInput(encoder);

        size_t output_buffer_size = 1000 * 1000 * 10;
        size_t out_buf_remain = output_buffer_size;
        uint8_t* output_buffer = (uint8_t *)malloc(output_buffer_size);
        uint8_t* out_buf_next = output_buffer;
        JxlEncoderStatus res = JxlEncoderProcessOutput(encoder, &out_buf_next, &out_buf_remain);
        
        /* Create image metadata before appending */
        Metadata new_meta = METADATA__INIT;
        new_meta.size = size;
        new_meta.width = width;
        new_meta.height = height;
        new_meta.channels = channels;
        new_meta.timestamp = timestamp;
        new_meta.bits_pixel = bits_pixel;
        new_meta.camera = camera;

        /* Append the image to the result batch */
        append_result_image(output_buffer, output_buffer_size - out_buf_remain, &new_meta);

        // Write encoded data to output file
        FILE* output_file = fopen("output.jxl", "wb");

        fwrite(output_buffer, 1, output_buffer_size - out_buf_remain, output_file);

        fclose(output_file);

        /* Remember to free any allocated memory */
        free(input_image_data);
        free(output_buffer);
        JxlEncoderDestroy(encoder);
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