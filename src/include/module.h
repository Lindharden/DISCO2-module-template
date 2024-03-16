#ifndef MODULE_H
#define MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


/**
 * Module main function. 
 * Will be called from parent process.
 *
 * @param input_batch Input image batch.
 * @param module_parameter_list Configuration parameter values.
 * 
 * @return resulting image batch
 */
ImageBatch run(ImageBatch *input_batch, ModuleParameterList *module_parameter_list);

// End extern "C" block
#ifdef __cplusplus
}
#endif

#endif // MODULE_H