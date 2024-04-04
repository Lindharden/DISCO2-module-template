#include "module.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "yaml_parser.h"

#define FILENAME_INPUT "input.png"
#define FILENAME_OUTPUT "output"
#define FILENAME_CONFIG "config.yaml"

void save_images(const char *filename_base, const ImageBatch *batch)
{
    uint32_t offset = 0;
    int image_index = 0;
    
    while (image_index < batch->num_images && offset < batch->batch_size) {
        uint32_t image_size = *((uint32_t *)(batch->data + offset));
        offset += sizeof(uint32_t); // Move the offset to the start of the image data

        char filename[20];
        sprintf(filename, "%s%d.png", filename_base, image_index);

        int stride = batch->width * batch->channels * sizeof(uint8_t);
        int success = stbi_write_png(filename, batch->width, batch->height, batch->channels, batch->data + offset, stride);
        if (!success)
        {
            fprintf(stderr, "[test.c]: Error writing image to %s\n", filename);
        }
        else
        {
            printf("[test.c]: Image saved as %s\n", filename);
        }

        offset += image_size; // Move the offset to the start of the next image block

        image_index++;
    }
}

void load_image(const char *filename, ImageBatch *batch, int num_images)
{
    int image_width, image_height, image_channels;
    unsigned char *image_data = stbi_load(filename, &image_width, &image_height, &image_channels, STBI_rgb_alpha);
    batch->height = image_height;
    batch->width = image_width;
    batch->channels = image_channels;
    batch->num_images = num_images;
    uint32_t image_size = image_height * image_width * image_channels;
    uint32_t batch_size = (image_size + sizeof(uint32_t)) * num_images;
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
        memcpy(batch->data + offset, &image_size, sizeof(uint32_t));
        offset += sizeof(uint32_t);
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
