#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "module_config.pb-c.h"

// GENERIC PROTOBUF UTIL FUNCTIONS //

ConfigParameter *get_param(ModuleConfig *config, const char *name) {
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