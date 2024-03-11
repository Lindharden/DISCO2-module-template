#include "types.h"

/**
 * Module main function. 
 * Will be called from parent process.
 *
 * @param input_batch Input image batch.
 * @param config Configuration parameter values.
 * 
 * @return resulting image batch
 */
ImageBatch run(ImageBatch *input_batch, ModuleConfig *config);