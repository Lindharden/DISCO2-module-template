#include "module.h"
#include "util.h"
#include <filesystem>
#include <opencv2/opencv.hpp>
namespace fs = std::filesystem;
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

        uchar *image_buffer;
        size_t size = get_image_data(i, &image_buffer);

        cv::Mat rawImage(metadata->height, metadata->width, CV_16UC1, image_buffer);
        cv::Mat demosaicedImage;
        cv::cvtColor(rawImage, demosaicedImage, cv::COLOR_BayerGR2RGB);
        demosaicedImage *= 16; // scale image to use 16 bits

        cv::Mat demosaicedImage_1byte;
        cv::normalize(demosaicedImage, demosaicedImage_1byte, 0, 255, 32, CV_8UC3);

        // fs::path dir ("./");
        // fs::path file ("image_" + std::to_string(std::time(0)) + "_" + std::to_string(i) + ".png");
        // std::string full_path = (dir / file).string();
        // imwrite(full_path, demosaicedImage);

        Metadata new_meta = METADATA__INIT;
        new_meta.channels = 3;
        new_meta.height = demosaicedImage_1byte.size().height;
        new_meta.width = demosaicedImage_1byte.size().width;
        new_meta.size = demosaicedImage_1byte.size().height*demosaicedImage_1byte.size().width*new_meta.channels;
        new_meta.timestamp = metadata->timestamp;
        new_meta.bits_pixel = 8;
        new_meta.camera = metadata->camera;

        /* Append the image to the result batch */
        append_result_image(demosaicedImage_1byte.data, new_meta.size, &new_meta);

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