#include "module.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define FILENAME_INPUT "input.png"
#define FILENAME_OUTPUT "output.png"

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
    size_t image_size = image_height * image_width * image_channels;
    batch->data = (unsigned char*)malloc(image_size);
    memcpy(batch->data, image_data, image_size);
}

ModuleConfig* module_configurations()
{
    ModuleConfig flip_config = MODULE_CONFIG__INIT;
    ConfigParameter flip_percent = CONFIG_PARAMETER__INIT;
    ConfigParameter flip_amount = CONFIG_PARAMETER__INIT;
    flip_percent.key = "flip_percent";
    flip_percent.value_case = CONFIG_PARAMETER__VALUE_FLOAT_VALUE;
    flip_percent.float_value = 0.20;
    flip_amount.key = "flip_amount";
    flip_amount.value_case = CONFIG_PARAMETER__VALUE_INT_VALUE;
    flip_amount.int_value = 1;
    flip_config.parameters = malloc(sizeof(ConfigParameter *) * 2);
    flip_config.n_parameters = 2;
    flip_config.parameters[0] = &flip_percent;
    flip_config.parameters[1] = &flip_amount;
    size_t lenConfig = module_config__get_packed_size(&flip_config);
    uint8_t bufConfig[lenConfig];
    module_config__pack(&flip_config, bufConfig);
    ModuleConfig *unpacked_config = module_config__unpack(NULL, lenConfig, bufConfig);
    return unpacked_config;
}

int main()
{
    ImageBatch batch;
    load_image(FILENAME_INPUT, &batch);

    ModuleConfig* config = module_configurations();
    
    ImageBatch result = run(&batch, config);

    save_image(FILENAME_OUTPUT, &result);
    free(config->parameters);
    free(result.data);
    return 0;
}
