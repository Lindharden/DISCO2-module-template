#include "module.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "yaml_parser.h"

#define FILENAME_INPUT "input.png"
#define FILENAME_OUTPUT "output.png"
#define FILENAME_CONFIG "config.yaml"

void save_images(const char *filename_base, const ImageBatch *batch)
{
    int image_size = batch->width * batch->height * batch->channels;
    for (size_t i = 0; i < batch->num_images; i++)
    {
        char filename[20];
        sprintf(filename, "%s%ld.png", filename_base, i);

        // Determine the desired output format (e.g., PNG)
        int stride = batch->width * batch->channels;
        int success = stbi_write_png(filename, batch->width, batch->height, batch->channels, &batch->data[i * image_size], stride);
        if (!success)
        {
            fprintf(stderr, "Error writing image to %s\n", filename);
        }
        else
        {
            printf("Image saved as %s\n", filename);
        }
    }
}

void load_image(const char *filename, ImageBatch *batch)
{
    int image_width, image_height, image_channels;
    unsigned char *image_data = stbi_load(filename, &image_width, &image_height, &image_channels, STBI_rgb_alpha);
    batch->height = image_height;
    batch->width = image_width;
    batch->channels = image_channels;
    batch->num_images = 1;
    batch->shm_key = 1;
    size_t image_size = image_height * image_width * image_channels;
    batch->data = (unsigned char*)malloc(image_size);
    memcpy(batch->data, image_data, image_size);
}

int main()
{
    ImageBatch batch;
    load_image(FILENAME_INPUT, &batch);

	ModuleConfig config;
	if (parse_module_yaml_file(FILENAME_CONFIG, &config) < 0)
		return -1;
    
    ImageBatch result = run(&batch, &config);

    save_images(FILENAME_OUTPUT, &result);
    free(config.parameters);
    return 0;
}
