#include "module.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "utils/yaml.c"
#include <sys/ipc.h>

#define FILENAME_INPUT "input.png"
#define FILENAME_OUTPUT "output.png"
#define FILENAME_CONFIG "test.yaml"

void save_image(const char *filename, const ImageBatch *batch)
{
    int stride = batch->width * batch->channels;
    int success = stbi_write_png(filename, batch->width, batch->height, batch->channels, batch->data, stride);
    if (!success)
    {
        fprintf(stderr, "Error writing image to %s\n", filename);
    }
    else
    {
        printf("Image saved as %s\n", filename);
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
    if (SHARED_MEMORY) {
        int shmid = shmget(batch->shm_key, image_size, IPC_CREAT | 0666);
        char *shmaddr = shmat(shmid, NULL, 0);
        memcpy(shmaddr, image_data, image_size);
    } else {
        batch->data = (unsigned char*)malloc(image_size);
        memcpy(batch->data, image_data, image_size);
    }
}

int main()
{
    ImageBatch batch;
    load_image(FILENAME_INPUT, &batch);

	ModuleConfig config = MODULE_CONFIG__INIT;
	if (parse_module_yaml_file(FILENAME_CONFIG, &config) < 0)
		return -1;
    
    ImageBatch result = run(&batch, &config);

    save_image(FILENAME_OUTPUT, &result);
    free(config.parameters);
    return 0;
}
