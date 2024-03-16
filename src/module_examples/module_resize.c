#include "module.h"
#include "util.h"

ImageBatch run(ImageBatch *input_batch, ModuleParameterList *config) {
    ImageBatch result_batch;
    // Correctly set the dimensions for the resized images
    result_batch.width = input_batch->width * 2;
    result_batch.height = input_batch->height * 2;
    result_batch.channels = input_batch->channels;
    result_batch.num_images = input_batch->num_images;

    // Calculate the size of the entire batch after resizing
    int output_size_per_image = result_batch.width * result_batch.height * result_batch.channels;
    unsigned char *new_data = (unsigned char *)malloc(output_size_per_image * result_batch.num_images);

    if (new_data == NULL)
    {
        fprintf(stderr, "[module_resize] Error: Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    result_batch.data = new_data;

    // Original size of one image in the batch
    int input_size_per_image = input_batch->width * input_batch->height * input_batch->channels;

    for (int i = 0; i < input_batch->num_images; ++i) {
        const unsigned char* input = input_batch->data + i * input_size_per_image;
        unsigned char* output = result_batch.data + i * output_size_per_image;
        resizeImageNearestNeighbor(input, input_batch->width, input_batch->height, input_batch->channels, output, result_batch.width, result_batch.height);
    }
    
    // Must be called
    finalize(input_batch, &result_batch);

    return result_batch;
}

void resizeImageNearestNeighbor(const unsigned char* input, int inputWidth, int inputHeight, int channels, unsigned char* output, int outputWidth, int outputHeight) {
    for (int y = 0; y < outputHeight; ++y) {
        for (int x = 0; x < outputWidth; ++x) {
            int srcX = x * inputWidth / outputWidth;
            int srcY = y * inputHeight / outputHeight;
            for (int c = 0; c < channels; ++c) {
                output[(y * outputWidth + x) * channels + c] = input[(srcY * inputWidth + srcX) * channels + c];
            }
        }
    }
}