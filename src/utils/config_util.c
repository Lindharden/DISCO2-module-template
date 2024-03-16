#include "util.h"

// GENERIC PROTOBUF UTIL FUNCTIONS //

static ModuleParameter *get_param(const char *name) {
    // find parameter
    ModuleParameter *found_parameter = NULL;
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

float get_param_float(const char *name)
{   
    ModuleParameter *found_parameter = get_param(name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != FLOAT_VALUE)
    {
        perror("Parameter not found or not of type float.");
    }
    
    return found_parameter->float_value;
}

int get_param_int(const char *name)
{   
    ModuleParameter *found_parameter = get_param(name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != INT_VALUE)
    {
        perror("Parameter not found or not of type int.");
    }
    
    return found_parameter->int_value;
}

int get_param_bool(const char *name)
{   
    ModuleParameter *found_parameter = get_param(name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != BOOL_VALUE)
    {
        perror("Parameter not found or not of type bool.");
    }
    
    return found_parameter->bool_value;
}

char *get_param_string(const char *name)
{   
    ModuleParameter *found_parameter = get_param(name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != STRING_VALUE)
    {
        perror("Parameter not found or not of type string.");
    }
    
    return found_parameter->string_value;
}