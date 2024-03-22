#include "util.h"

static ModuleParameter *get_param(const char *name) {
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

int get_param_bool(const char *name)
{   
    ModuleParameter *found_parameter = get_param(name);

    if (found_parameter == NULL || found_parameter->value_case != BOOL_VALUE)
    {
        signal_error_and_exit(503);
    }
    
    return found_parameter->bool_value;
}

int get_param_int(const char *name)
{   
    ModuleParameter *found_parameter = get_param(name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != INT_VALUE)
    {
        signal_error_and_exit(504);
    }
    
    return found_parameter->int_value;
}

float get_param_float(const char *name)
{   
    ModuleParameter *found_parameter = get_param(name);

    if (found_parameter == NULL || found_parameter->value_case != FLOAT_VALUE)
    {
        signal_error_and_exit(505);
    }
    
    return found_parameter->float_value;
}

char *get_param_string(const char *name)
{   
    ModuleParameter *found_parameter = get_param(name);

    // identify value type
    if (found_parameter == NULL || found_parameter->value_case != STRING_VALUE)
    {
        signal_error_and_exit(506);
    }
    
    return found_parameter->string_value;
}