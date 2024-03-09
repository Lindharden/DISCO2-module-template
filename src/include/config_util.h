#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "module_config.pb-c.h"

// GENERIC PROTOBUF UTIL FUNCTIONS //

/**
 * Search the module configuration and fetch parameter 
 * with specified name.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 */
static ConfigParameter *get_param(ModuleConfig *config, const char *name) {
    // find parameter
    ConfigParameter *found_parameter = NULL;
    for (size_t i = 0; i < config->n_parameters; i++)
    {
        if (strcmp(config->parameters[i]->key, name) == 0)
        {
            found_parameter = config->parameters[i];
            break;
        }
    }
    return found_parameter;
}

/**
 * Fetch float parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 * 
 * @return float value of parameter
 */
float get_param_float(ModuleConfig *config, const char *name)
{   
    ConfigParameter *found_parameter = get_param(config, name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != CONFIG_PARAMETER__VALUE_FLOAT_VALUE)
    {
        perror("Parameter not found or not of type float.");
    }
    
    return found_parameter->float_value;
}

/**
 * Fetch integer parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 *
 * @return int value of parameter
 */
int get_param_int(ModuleConfig *config, const char *name)
{   
    ConfigParameter *found_parameter = get_param(config, name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != CONFIG_PARAMETER__VALUE_INT_VALUE)
    {
        perror("Parameter not found or not of type int.");
    }
    
    return found_parameter->int_value;
}

/**
 * Fetch boolean parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 * 
 * @return bool value of parameter as integer (0, 1)
 */
int get_param_bool(ModuleConfig *config, const char *name)
{   
    ConfigParameter *found_parameter = get_param(config, name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != CONFIG_PARAMETER__VALUE_BOOL_VALUE)
    {
        perror("Parameter not found or not of type bool.");
    }
    
    return found_parameter->bool_value;
}

/**
 * Fetch string parameter from module configuration.
 *
 * @param config Pointer to the module configuration
 * @param name Name of the desired parameter
 *
 * @return string value of parameter
 */
char *get_param_string(ModuleConfig *config, const char *name)
{   
    ConfigParameter *found_parameter = get_param(config, name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != CONFIG_PARAMETER__VALUE_STRING_VALUE)
    {
        perror("Parameter not found or not of type string.");
    }
    
    return found_parameter->string_value;
}