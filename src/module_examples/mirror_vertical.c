#include "module.h"
#include "util.h"

/* Define custom error codes in the range of 1-99*/
enum ERROR_CODE {
    MALLOC_ERR = 1,
    PLACEHOLDER = 2,
};

void module()
{
    float flip_percent = get_param_float("flip_percent");

    for (int i = 0; i < get_input_num_images(); ++i)
    {
        Metadata *input_meta = get_metadata(i);
        int size = input_meta->size;
        int height = input_meta->height;
        int width = input_meta->width;
        int channels = input_meta->channels;
        int timestamp = input_meta->timestamp;
        int bits_pixel = input_meta->bits_pixel;
        char *camera = input_meta->camera;

        int flip_height = height * flip_percent;

        unsigned char *input_image_data;
        get_image_data(i, &input_image_data);
        unsigned char *output_image_data = (unsigned char *)malloc(size);
        if (output_image_data == NULL)
        {
            signal_error_and_exit(MALLOC_ERR);
        }

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width / 2; ++x)
            {
                // Swap pixel values horizontally
                for (int c = 0; c < channels; ++c)
                {
                    if (y > flip_height)
                    {
                        output_image_data[((y * width + x) * channels + c)] =
                            input_image_data[(y * width + x) * channels + c];
                        output_image_data[((y * width + (width - 1 - x)) * channels + c)] =
                            input_image_data[(y * width + (width - 1 - x)) * channels + c];
                    }
                    else
                    {
                        unsigned char temp = input_image_data[(y * width + x) * channels + c];
                        output_image_data[((y * width + x) * channels + c)] =
                            input_image_data[(y * width + (width - 1 - x)) * channels + c];
                        output_image_data[((y * width + (width - 1 - x)) * channels + c)] = temp;
                    }
                }
            }
        }  

        Metadata new_meta = METADATA__INIT;
        new_meta.size = size;
        new_meta.width = width;
        new_meta.height = height;
        new_meta.channels = channels;
        new_meta.timestamp = timestamp;
        new_meta.bits_pixel = bits_pixel;
        new_meta.camera = camera;

        append_result_image(output_image_data, size, &new_meta);
        free(input_image_data);
        free(output_image_data);
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
    unpack_metadata();

    module();

    finalize();

    return result_batch;
}