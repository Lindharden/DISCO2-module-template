#include "module.h"
#include "util.h"

/* Define custom error codes in the range of 1-99*/
enum ERROR_CODE {
    MALLOC_ERR = 1,
    PLACEHOLDER = 2,
};

void module()
{
    int input_width = get_input_width();
    int input_height = get_input_height();
    int input_channels = get_input_channels();

    set_result_dimensions(input_width, input_height, input_channels);
    set_result_num_images(get_input_num_images());

    float flip_percent = get_param_float("flip_percent");
    int flip_height = (int)(get_input_height() * flip_percent);

    for (int i = 0; i < get_input_num_images(); ++i)
    {
        unsigned char *input_image_data;
        uint32_t img_size = get_image_data(i, &input_image_data);
        unsigned char *output_image_data = (unsigned char *)malloc(img_size);
        if (output_image_data == NULL)
        {
            signal_error_and_exit(MALLOC_ERR);
        }

        for (int y = 0; y < input_height; ++y)
        {
            for (int x = 0; x < input_width / 2; ++x)
            {
                // Swap pixel values horizontally
                for (int c = 0; c < input_channels; ++c)
                {
                    if (y > flip_height)
                    {
                        output_image_data[((y * input_width + x) * input_channels + c)] =
                            input_image_data[(y * input_width + x) * input_channels + c];
                        output_image_data[((y * input_width + (input_width - 1 - x)) * input_channels + c)] =
                            input_image_data[(y * input_width + (input_width - 1 - x)) * input_channels + c];
                    }
                    else
                    {
                        unsigned char temp = input_image_data[(y * input_width + x) * input_channels + c];
                        output_image_data[((y * input_width + x) * input_channels + c)] =
                            input_image_data[(y * input_width + (input_width - 1 - x)) * input_channels + c];
                        output_image_data[((y * input_width + (input_width - 1 - x)) * input_channels + c)] = temp;
                    }
                }
            }
        }

        append_result_image(output_image_data, img_size);
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

    module();

    finalize();

    return result_batch;
}