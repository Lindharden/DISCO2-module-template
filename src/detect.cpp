#include "module.h"
#include "util.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/tools/gen_op_registration.h"
#include <opencv2/opencv.hpp>

/* Define custom error codes */
enum ERROR_CODE {
    MALLOC_ERR = 1,
    PLACEHOLDER = 2,
};

/* START MODULE IMPLEMENTATION */
void module()
{
    for(int i = 0; i < input->num_images; i++){
        Metadata *metadata = get_metadata(i);
        int height = metadata->height;
        int width = metadata->width;
        int channels = metadata->channels;
        int timestamp = metadata->timestamp;
        int bits_pixel = metadata->bits_pixel;
        char *camera = metadata->camera;

        uchar *image_buffer;
        size_t size = get_image_data(i, &image_buffer);
        unsigned char *output_image_data; 

        // Ensure bits per pixel is compatible with OpenCV's CV_8U (8 bits unsigned) for simplicity
        int outputSize;
        if(bits_pixel == 8 && (channels == 1 || channels == 3 || channels == 4)) {
            int type;
            if(channels == 1) type = CV_8UC1;
            else if(channels == 3) type = CV_8UC3;
            else if(channels == 4) type = CV_8UC4;

            cv::Mat image(height, width, type, image_buffer);

            cv::Mat greyImage;
            cv::cvtColor(image, greyImage, cv::COLOR_BGR2GRAY);

            outputSize = greyImage.rows * greyImage.cols;
            output_image_data = (unsigned char *)malloc(outputSize);

            memcpy(output_image_data, greyImage.data, outputSize);

        } else {
            std::cerr << "Unsupported image format." << std::endl;
        }

        /* Check for malloc error */
        if (output_image_data == NULL)
        {
            signal_error_and_exit(MALLOC_ERR);
        }

        std::unique_ptr<tflite::Interpreter> interpreter = std::make_unique<tflite::Interpreter>();
        // NOT FULLY IMPLEMENTED
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

        /* Append the image to the result batch */
        append_result_image(image_buffer, new_meta.size, &new_meta);

        delete[] image_buffer;
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