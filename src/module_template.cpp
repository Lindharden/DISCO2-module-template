#include "module.h"
#include "util.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/tools/gen_op_registration.h"
#include "opencv2/opencv.hpp"

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
        int size = input_meta->size;
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
        get_image_data(i, &input_image_data);

        unsigned char *output_image_data; 
        
        // Ensure bits per pixel is compatible with OpenCV's CV_8U (8 bits unsigned) for simplicity
        if(bits_pixel == 8 && (channels == 1 || channels == 3 || channels == 4)) {
            int type;
            if(channels == 1) type = CV_8UC1;
            else if(channels == 3) type = CV_8UC3;
            else if(channels == 4) type = CV_8UC4;

            cv::Mat image(height, width, type, input_image_data);

            cv::Mat greyImage;
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

            int outputSize = grayImage.rows * grayImage.cols;
            output_image_data = (unsigned char *)malloc(outputSize);

            memcopy(output_image_data, grayImage.data, outputSize);

        } else {
            std::cerr << "Unsupported image format." << std::endl;
        }

        /* Check for malloc error */
        if (output_image_data == NULL)
        {
            signal_error_and_exit(MALLOC_ERR);
        }

        std::unique_ptr<tflite::Interpreter> interpreter = std::make_unique<tflite::Interpreter>();
        std::cout << "test" << interpreter->tensors_size() << std::endl;

        /* Create image metadata before appending */
        Metadata new_meta = METADATA__INIT;
        new_meta.size = outputSize;
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
        append_result_image(output_image_data, outputSize, &new_meta);

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