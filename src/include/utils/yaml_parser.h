#include <yaml.h>
#include <errno.h>
#include <math.h>
#include "module_config.pb-c.h"

/**
 * Parse contents of yaml file into ModuleConfig.
 *
 * @param filename yaml file name (relative path)
 * @param module_config output variable
 *
 * @return status code (0 = success, -1 = failure)
 */
int parse_module_yaml_file(const char *filename, ModuleConfig *module_config);