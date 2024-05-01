#include <stdio.h>
#include <stdlib.h>
#include <jxl/decode.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    const char* input_filename = "output.jxl";
    const char* output_filename = "output.png";

    // Create JxlDecoder instance
    JxlDecoder* decoder = JxlDecoderCreate(NULL);

    // Set input file
    FILE* input_file = fopen(input_filename, "rb");
    // Get size of input file
    fseek(input_file, 0, SEEK_END);
    long input_file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    uint8_t* input_buffer = (uint8_t*)malloc(input_file_size);
    fread(input_buffer, 1, input_file_size, input_file);
    fclose(input_file);

    JxlDecoderStatus res1 = JxlDecoderSetInput(decoder, input_buffer, input_file_size);

    if (res1 == JXL_DEC_ERROR)
    {
        printf("JXL_DEC_ERROR1");
        return 1;
    }

    JxlBasicInfo basic_info;
    size_t buffer_size;
    uint8_t* decoded_data;
    JxlPixelFormat format;
    uint8_t channels;
    JxlDecoderStatus res2 = JxlDecoderSubscribeEvents(decoder, JXL_DEC_BASIC_INFO | JXL_DEC_FULL_IMAGE | JXL_DEC_BASIC_INFO);
    
    while (1)
    {
        JxlDecoderStatus status = JxlDecoderProcessInput(decoder);

        if (status == JXL_DEC_ERROR)
        {
            printf("JXL_DEC_ERROR2");
            break;
        }

        if (status == JXL_DEC_SUCCESS) 
        {
            break;
        }
        
        if (status == JXL_DEC_FULL_IMAGE) 
        {
            break;
        }

        if (status == JXL_DEC_BASIC_INFO) 
        {
            JxlDecoderStatus res3 = JxlDecoderGetBasicInfo(decoder, &basic_info);
            channels = basic_info.num_color_channels + basic_info.num_extra_channels;
            format.num_channels = channels; format.data_type = JXL_TYPE_UINT8; 
            format.endianness = JXL_NATIVE_ENDIAN; format.align = 0;
        }
        
        if (status == JXL_DEC_NEED_IMAGE_OUT_BUFFER) 
        {
            JxlDecoderStatus res4 = JxlDecoderImageOutBufferSize(decoder, &format, &buffer_size);
            decoded_data = (uint8_t *)malloc(buffer_size);
            JxlDecoderStatus res5 = JxlDecoderSetImageOutBuffer(decoder, &format, decoded_data, buffer_size);
        }
    }

    stbi_write_png(output_filename, basic_info.xsize, basic_info.ysize, channels, decoded_data, basic_info.xsize * channels);

    free(decoded_data);
    free(input_buffer);
    
    JxlDecoderDestroy(decoder);

    printf("Image decoding successful: %s -> %s\n", input_filename, output_filename);

    return 0;
}
