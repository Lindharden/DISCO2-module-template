#include <yaml.h>
#include <errno.h>
#include <math.h>
#include "types.h"

/**
 * Parse contents of yaml file into ModuleConfig.
 *
 * @param filename yaml file name (relative path)
 * @param module_parameter_list output variable
 *
 * @return status code (0 = success, -1 = failure)
 */
int parse_module_yaml_file(const char *filename, ModuleParameterList *module_parameter_list);