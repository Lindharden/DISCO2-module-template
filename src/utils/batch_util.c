#include "util.h"
#include <sys/shm.h>

ImageBatch *input;
ImageBatch *result;
ModuleParameterList *config;

int get_input_num_images()
{
    return input->num_images;
}

size_t get_image_data(int index, unsigned char **out)
{
    Metadata *image_meta = get_metadata(index);
    *out = (unsigned char *)malloc(image_meta->size);
    if (out == NULL)
    {
        signal_error_and_exit(100);
    }
    memcpy(*out, input->data + image_meta->image_offset, image_meta->size);
    return image_meta->size;
}

void append_result_image(unsigned char *data, uint32_t data_size, Metadata *meta)
{
    /* Pack new metadata */
    size_t meta_size = metadata__get_packed_size(meta);
    uint8_t meta_buf[meta_size];
    metadata__pack(meta, meta_buf);
    size_t block_size = data_size + meta_size + sizeof(uint32_t);
    if (result->batch_size == 0)
    {
        result->data = (unsigned char *)malloc(block_size);

        if (result->data == NULL)
        {
            signal_error_and_exit(100);
        }
    }
    else
    {
        unsigned char *tmp = (unsigned char *)realloc(result->data, result->batch_size + block_size);

        if (tmp == NULL)
        {
            signal_error_and_exit(101);
        }

        result->data = tmp;
    }

    if (result->data == NULL)
        return;

    /* Insert meta size, then the metadata, then the image data */
    unsigned char *ptr = result->data + result->batch_size;
    memcpy(ptr, &meta_size, sizeof(uint32_t));
    ptr += sizeof(uint32_t);
    memcpy(ptr, meta_buf, meta_size);
    ptr += meta_size;
    memcpy(ptr, data, data_size);

    result->batch_size += block_size;
    result->num_images += 1;
}

static void attach()
{
    void *shmaddr = shmat(input->shmid, NULL, 0);
    if (shmaddr == NULL)
    {
        signal_error_and_exit(303);
    }
    input->data = shmaddr;
}

void initialize()
{
    result->batch_size = 0;
    result->num_images = 0;
    result->pipeline_id = input->pipeline_id;
    if (SHARED_MEMORY) attach();
    unpack_metadata();
}