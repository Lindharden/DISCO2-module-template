#include "util.h"

// GENERIC PROTOBUF UTIL FUNCTIONS //

static ModuleParameter *get_param(ModuleParameterList *config, const char *name) {
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

float get_param_float(ModuleParameterList *config, const char *name)
{   
    ModuleParameter *found_parameter = get_param(config, name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != FLOAT_VALUE)
    {
        perror("Parameter not found or not of type float.");
    }
    
    return found_parameter->float_value;
}

int get_param_int(ModuleParameterList *config, const char *name)
{   
    ModuleParameter *found_parameter = get_param(config, name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != INT_VALUE)
    {
        perror("Parameter not found or not of type int.");
    }
    
    return found_parameter->int_value;
}

int get_param_bool(ModuleParameterList *config, const char *name)
{   
    ModuleParameter *found_parameter = get_param(config, name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != BOOL_VALUE)
    {
        perror("Parameter not found or not of type bool.");
    }
    
    return found_parameter->bool_value;
}

char *get_param_string(ModuleParameterList *config, const char *name)
{   
    ModuleParameter *found_parameter = get_param(config, name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != STRING_VALUE)
    {
        perror("Parameter not found or not of type string.");
    }
    
    return found_parameter->string_value;
}