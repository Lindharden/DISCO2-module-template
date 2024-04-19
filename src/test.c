#include "module.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "yaml_parser.h"
#include "metadata.pb-c.h"

#define FILENAME_INPUT "input.png"
#define FILENAME_OUTPUT "output"
#define FILENAME_CONFIG "config.yaml"

void save_images(const char *filename_base, const ImageBatch *batch)
{
    uint32_t offset = 0;
    int image_index = 0;

    while (image_index < batch->num_images && offset < batch->batch_size)
    {
        uint32_t meta_size = *((uint32_t *)(batch->data + offset));
        offset += sizeof(uint32_t); // Move the offset to the start of metadata
        Metadata *metadata = metadata__unpack(NULL, meta_size, batch->data + offset);
        offset += meta_size; // Move offset to start of image

        char filename[20];
        sprintf(filename, "%s%d.png", filename_base, image_index);

        int stride = metadata->width * metadata->channels * sizeof(uint8_t);
        int success = stbi_write_png(filename, metadata->width, metadata->height, metadata->channels, batch->data + offset, stride);
        if (!success)
        {
            fprintf(stderr, "Error writing image to %s\n", filename);
        }
        else
        {
            printf("Image saved as %s\n", filename);
        }

        offset += metadata->size; // Move the offset to the start of the next image block

        image_index++;
    }
}

void load_image(const char *filename, ImageBatch *batch, int num_images)
{
    int image_width, image_height, image_channels;
    unsigned char *image_data = stbi_load(filename, &image_width, &image_height, &image_channels, STBI_rgb_alpha);
    batch->num_images = num_images;
    uint32_t image_size = image_height * image_width * image_channels;
    Metadata new_meta = METADATA__INIT;
    new_meta.size = image_size;
    new_meta.width = image_width;
    new_meta.height = image_height;
    new_meta.channels = image_channels;
    new_meta.timestamp = 0; // example time (should be using unix timestamp)
    new_meta.bits_pixel = 8;
    new_meta.camera = "rgb";
    size_t meta_size = metadata__get_packed_size(&new_meta);
    uint8_t meta_buf[meta_size];
    metadata__pack(&new_meta, meta_buf);
    uint32_t batch_size = (image_size + sizeof(uint32_t) + meta_size) * num_images;
    batch->data = (unsigned char*)malloc(batch_size);

    if (batch->data == NULL)
    {
        fprintf(stderr, "[test] Error: Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    batch->batch_size = batch_size;
    int offset = 0;
    for (uint32_t i = 0; i < num_images; i++)
    {
        memcpy(batch->data + offset, &meta_size, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        memcpy(batch->data + offset, &meta_buf, meta_size);
        offset += meta_size;
        memcpy(batch->data + offset, image_data, image_size);
        offset += image_size;
    }
}

int main(int argc, char *argv[])
{
    int num_images = 1;
    if (argc > 1)
    {
        num_images = atoi(argv[1]);
    }
    ImageBatch batch;
    load_image(FILENAME_INPUT, &batch, num_images);

	ModuleParameterList module_parameter_list;
	if (parse_module_yaml_file(FILENAME_CONFIG, &module_parameter_list) < 0)
		return -1;

    ImageBatch result = run(&batch, &module_parameter_list, NULL);

    save_images(FILENAME_OUTPUT, &result);
    free(module_parameter_list.parameters);
    free(result.data);
    return 0;
}
