#include "util.h"

ImageBatch *input;
ImageBatch *result;
ModuleParameterList *config;

int get_input_width()
{
    return input->width;
}

int get_input_height()
{
    return input->height;
}

int get_input_channels()
{
    return input->channels;
}

int get_input_num_images()
{
    return input->num_images;
}

size_t get_image_data(int index, unsigned char **out)
{
    if (index > input->num_images || index < 0)
        return 0;
    size_t offset = 0;
    int image_index = 0;

    while (image_index < input->num_images && offset < input->batch_size)
    {
        size_t image_size = *((size_t *)(input->data + offset));
        offset += sizeof(size_t); // Move the offset to the start of the image data

        // Extract the image data
        if (image_index == index)
        {
            *out = (unsigned char *)malloc(image_size);

            if (*out == NULL) {
                fprintf(stderr, "[Batch_util] Error: Unable to allocate memory.\n");
                exit(EXIT_FAILURE);
            }

            memcpy(*out, input->data + offset, image_size);
            return image_size;
        }

        offset += image_size; // Move the offset to the start of the next image block

        image_index++;
    }
    return 0;
}

void set_result_width(int width)
{
    result->width = width;
}

void set_result_height(int height)
{
    result->height = height;
}

void set_result_channels(int channels)
{
    result->channels = channels;
}

void set_result_dimensions(int width, int height, int channels)
{
    set_result_width(width);
    set_result_height(height);
    set_result_channels(channels);
}

void set_result_num_images(int num_images)
{
    result->num_images = num_images;
}

void append_result_image(unsigned char *data, size_t data_size)
{
    if (result->batch_size == 0)
    {
        result->data = (unsigned char *)malloc(data_size + sizeof(size_t));
        
        if (result->data == NULL)
        {
            fprintf(stderr, "[Batch_util] Error: Unable to allocate memory.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        unsigned char *tmp = (unsigned char *)realloc(result->data, result->batch_size + data_size + sizeof(size_t));

        if (tmp == NULL) 
        {
            fprintf(stderr, "[Batch_util] Error: Unable to reallocate memory.\n");
            exit(EXIT_FAILURE);
        }

        result->data = tmp;
    }

    if (result->data == NULL)
        return;

    unsigned char *ptr = result->data + result->batch_size;
    memcpy(ptr, &data_size, sizeof(size_t));
    ptr += sizeof(size_t); // insert after size
    memcpy(ptr, data, data_size);
    result->batch_size += data_size + sizeof(size_t);
}