#include "module.h"
#include "util.h"

ImageBatch run(ImageBatch *input_batch, ModuleParameterList *config) {
    // Create a new ImageBatch object to store the black and white image
    ImageBatch gray_batch;
    gray_batch.mtype = input_batch->mtype;
    gray_batch.height = input_batch->height;
    gray_batch.width = input_batch->width;
    gray_batch.channels = 1; // Black and white image will have only one channel
    gray_batch.num_images = input_batch->num_images;

    int batch_size = input_batch->width * input_batch->height * input_batch->channels * input_batch->num_images;

    // Calculate the size of the black and white image data
    size_t bwDataSize = gray_batch.height * gray_batch.width * gray_batch.channels * gray_batch.num_images;
    unsigned char *new_data = (unsigned char *)malloc(bwDataSize);
    gray_batch.data = new_data;

    for (int i = 0; i < batch_size; i += input_batch->channels) {
        // Extract color components from the original pixel
        unsigned char r = input_batch->data[i];
        unsigned char g = input_batch->data[i + 1];
        unsigned char b = input_batch->data[i + 2];

        // Convert to grayscale using luminance method (0.299R + 0.587G + 0.114B)
        unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

        new_data[i / input_batch->channels] = gray;
    }

    finalize(input_batch, &gray_batch);

    return gray_batch;
}