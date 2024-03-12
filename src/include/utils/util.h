#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "types.h"

// PROTOBUF UTILITY FUNCTIONS //

/**
 * Fetch float parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 * 
 * @return float value of parameter
 */
float get_param_float(ModuleParameterList *config, const char *name);

/**
 * Fetch integer parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 *
 * @return int value of parameter
 */
int get_param_int(ModuleParameterList *config, const char *name);

/**
 * Fetch boolean parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 * 
 * @return bool value of parameter as integer (0, 1)
 */
int get_param_bool(ModuleParameterList *config, const char *name);

/**
 * Fetch string parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 *
 * @return string value of parameter
 */
char *get_param_string(ModuleParameterList *config, const char *name);


// MODULE UTILITY FUNCTIONS //

/**
 * Finalize the resulting image batch data.
 * Copies to new shared memory segment if needed.
 *
 * @param input_batch Pointer to the old image batch.
 * @param result_batch Pointer to the resulting image batch.
 */
void finalize(ImageBatch *input_batch, ImageBatch *result_batch);