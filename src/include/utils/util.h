#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "types.h"
#include "module.h"
#include "globals.h"

// PROTOBUF UTILITY FUNCTIONS //

/**
 * Fetch float parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 * 
 * @return float value of parameter
 */
float get_param_float(const char *name);

/**
 * Fetch integer parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 *
 * @return int value of parameter
 */
int get_param_int(const char *name);

/**
 * Fetch boolean parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 * 
 * @return bool value of parameter as integer (0, 1)
 */
int get_param_bool(const char *name);

/**
 * Fetch string parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 *
 * @return string value of parameter
 */
char *get_param_string(const char *name);


// MODULE UTILITY FUNCTIONS //

/**
 * Finalize the resulting image batch data.
 * Copies to new shared memory segment if needed.
 */
void finalize();


// BATCH DATA UTILITY FUNCTIONS //

/**
 * Fetch width of the input image from the module configuration.
 *
 * @return Width of the input image
 */
int get_input_width();

/**
 * Fetch height of the input image from the module configuration.
 *
 * @return Height of the input image
 */
int get_input_height();

/**
 * Fetch number of channels of the input image from the module configuration.
 *
 * @return Number of channels of the input image
 */
int get_input_channels();

/**
 * Fetch number of images in the input batch from the module configuration.
 *
 * @return Number of images in the input batch
 */
int get_input_num_images();

/**
 * Retrieves the data of an image at the specified index, allocating memory and returning the size of the image data.
 *
 * @param index Index of image
 * @param out Buffer to store image data in
 * @return Size of image data as uint32_t
 */
uint32_t get_image_data(int index, unsigned char **out);

/**
 * Set the width of the resulting image in the module configuration.
 *
 * @param width Width of the resulting image
 */
void set_result_width(int width);

/**
 * Set the height of the resulting image in the module configuration.
 *
 * @param height Height of the resulting image
 */
void set_result_height(int height);

/**
 * Set the number of channels of the resulting image in the module configuration.
 *
 * @param channels Number of channels of the resulting image
 */
void set_result_channels(int channels);

/**
 * Set the dimensions (width, height, and channels) of the resulting image in the module configuration.
 *
 * @param width Width of the resulting image
 * @param height Height of the resulting image
 * @param channels Number of channels of the resulting image
 */
void set_result_dimensions(int width, int height, int channels);

/**
 * Set the number of images in the resulting batch in the module configuration.
 *
 * @param num_images Number of images in the resulting batch
 */
void set_result_num_images(int num_images);

/**
 * Append an image to the resulting batch in the module configuration.
 *
 * @param data Pointer to the image data
 * @param data_size Size of the image data
 */
void append_result_image(unsigned char *data, uint32_t data_size);


// ERROR REPORTING UTILITY FUNCTIONS //

/**
 * Report an error to the main process and exit with failure
 * 
 * @param error_code code of the error (0-99)
*/
void signal_error_and_exit(uint16_t error_code);

// End extern "C" block
#ifdef __cplusplus
}
#endif