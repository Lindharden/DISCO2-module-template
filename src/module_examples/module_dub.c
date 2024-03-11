#include "module.h"
#include "util.h"

/* Main function of module */
ImageBatch run(ImageBatch *input_batch, ModuleConfig *config) {
    /* Create a result batch, replace with expected dimensions */
    ImageBatch result_batch;
    result_batch.width = input_batch->width;
    result_batch.height = input_batch->height;
    result_batch.channels = input_batch->channels;
    result_batch.num_images = input_batch->num_images * 2;

    /* Allocate memory for resulting image data, fit to expected output size */
    int img_size = result_batch.width * result_batch.height * result_batch.channels;
    int data_size = img_size * result_batch.num_images;
    unsigned char *new_data = (unsigned char *)malloc(data_size);
    result_batch.data = new_data;

    for (size_t i = 0; i < result_batch.num_images; i++)
    {
        memcpy(&new_data[i * img_size], input_batch->data, img_size);
    }
    
    /* Finalize the module job (MUST BE HERE!) */
    finalize(input_batch, &result_batch);

    return result_batch;
}