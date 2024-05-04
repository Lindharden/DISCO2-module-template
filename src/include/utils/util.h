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
#include "metadata.pb-c.h"

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


// METADATA AND IMAGE DATA UTILITY FUNCTIONS //

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
 * @return Size of image data
 */
size_t get_image_data(int index, unsigned char **out);

/**
 * Retrieves the metadata of an image at the specified index, allocating memory and returning the size of the metadata.
 *
 * @param index Index of image
 * @return Size of metadata
 */
Metadata *get_metadata(int index);

/**
 * Unpack and cache metadata
*/
void unpack_metadata();

/**
 * Add custom key-value pair to image metadata, of type bool
 *
 * @param data Metadata that is to be added to
 * @param key The name associated with the custom value
 * @param val The boolean value to store
 */
void add_custom_metadata_bool(Metadata *data, char *key, int val);

/**
 * Add custom key-value pair to image metadata, of type integer
 *
 * @param data Metadata that is to be added to
 * @param key The name associated with the custom value
 * @param val The integer value to store
 */
void add_custom_metadata_int(Metadata *data, char *key, int val);

/**
 * Add custom key-value pair to image metadata, of type float
 *
 * @param data Metadata that is to be added to
 * @param key The name associated with the custom value
 * @param val The float value to store
 */
void add_custom_metadata_float(Metadata *data, char *key, float val);

/**
 * Add custom key-value pair to image metadata, of type string
 *
 * @param data Metadata that is to be added to
 * @param key The name associated with the custom value
 * @param val The string value to store
 */
void add_custom_metadata_string(Metadata *data, char *key, char *val);

/**
 * Get custom metadata value of type bool
 *
 * @param data Metadata to get from
 * @param key The name associated with the custom value
 */
int get_custom_metadata_bool(Metadata *data, char *key);

/**
 * Get custom metadata value of type int
 *
 * @param data Metadata to get from
 * @param key The name associated with the custom value
 */
int get_custom_metadata_int(Metadata *data, char *key);

/**
 * Get custom metadata value of type float
 *
 * @param data Metadata to get from
 * @param key The name associated with the custom value
 */
float get_custom_metadata_float(Metadata *data, char *key);

/**
 * Get custom metadata value of type string
 *
 * @param data Metadata to get from
 * @param key The name associated with the custom value
 */
char *get_custom_metadata_string(Metadata *data, char *key);

/**
 * Append an image to the resulting batch in the module configuration.
 *
 * @param data Pointer to the image data
 * @param data_size Size of the image data
 * @param new_meta Pointer to the metadata
 */
void append_result_image(unsigned char *data, uint32_t data_size, Metadata *new_meta);

/**
 * Initialize module globals
*/
void initialize();


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