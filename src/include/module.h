#include "types.h"

/**
 * Module main function. 
 * Will be called from parent process.
 *
 * @param input_batch Input image batch data.
 * @param config Configuration parameter values.
 */
ImageBatch run(ImageBatch *input_batch, ModuleConfig *config);