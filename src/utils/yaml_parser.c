#include "yaml_parser.h"
#include <limits.h>

static int initialize_parser(const char *filename, yaml_parser_t *parser, FILE *fh)
{
    fh = fopen(filename, "r");

    /* Initialize parser */
    if (!yaml_parser_initialize(parser))
    {
        fprintf(stderr, "Error: Failed to initialize parser!\n");
        return -1;
    }
    if (fh == NULL)
    {
        fprintf(stderr, "Error: Failed to open file!\n");
        return -1;
    }

    /* Set input file */
    yaml_parser_set_input_file(parser, fh);

    return 0;
}

static void cleanup_resources(yaml_parser_t *parser, yaml_event_t *event, FILE *fh)
{
    if (event != NULL)
    {
        yaml_event_delete(event);
    }
    yaml_parser_delete(parser);

    if (fh != NULL)
    {
        fclose(fh);
    }
}

static int safe_atof(const char *in, float *out)
{
    errno = 0; // To detect overflow or underflow
    char *endptr;
    float val = strtof(in, &endptr);

    if (endptr == in)
    {
        fprintf(stderr, "Error: Value \"%s\" could not be parsed to a floating-point number.\n", in);
        return -1;
    }
    else if (*endptr != '\0')
    {
        fprintf(stderr, "Error: Extra characters after number: \"%s\"\n", endptr);
        return -1;
    }
    else if ((val == HUGE_VALF || val == -HUGE_VALF) && errno == ERANGE)
    {
        // Note: Checking against HUGE_VALF for float-specific overflow/underflow
        fprintf(stderr, "Error: Value \"%s\" is out of range.\n", in);
        return -1;
    }

    *out = val;
    return 0;
}

static int safe_atoi(const char *in, int *out)
{
    errno = 0; // To detect overflow
    char *endptr;
    long val = strtol(in, &endptr, 10); // Base 10 for decimal conversion

    if (endptr == in)
    {
        fprintf(stderr, "Error: No digits were found in token %s\n", in);
        return -1;
    }
    if (*endptr != '\0')
    {
        fprintf(stderr, "Error: Extra characters after number: %s in token %s\n", endptr, in);
        return -1;
    }
    if ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE)
    {
        fprintf(stderr, "Error: Value out of long int range for token %s\n", in);
        return -1;
    }
    if (val < INT_MIN || val > INT_MAX)
    {
        fprintf(stderr, "Error: Value out of int range for token %s\n", in);
        return -1;
    }

    *out = (int)val;
    return 0;
}

int parse_module_yaml_file(const char *filename, ModuleParameterList *module_parameter_list)
{
    yaml_parser_t parser;
    FILE *fh = NULL;
    if (initialize_parser(filename, &parser, fh) < 0)
        return -1;

    /* Module definitions */
    ModuleParameter *params = NULL;
    int param_idx = -1;  // current module index
    int param_count = 0; // find total amount of modules

    /* START parsing */
    yaml_event_t event;
    while (1)
    {
        if (!yaml_parser_parse(&parser, &event))
        {
            printf("Error: Parser error %d\n", parser.error);
            return -1;
        }
        switch (event.type)
        {
            case YAML_MAPPING_START_EVENT:
                // New Dash
                param_idx++;
                ModuleParameter *temp = realloc(params, (param_count + 1) * sizeof(ModuleParameter));
                if (!temp)
                {
                    fprintf(stderr, "Error: Failed to allocate memory for ModuleParameter during parsing\n");
                    return -1;
                }
                params = temp;

                param_count++;
                break;
            case YAML_SCALAR_EVENT:
                // New set of ModuleDefinition encountered
                if (strcmp((char *)event.data.scalar.value, "key") == 0)
                {
                    // Expect the next event to be the value of order
                    if (!yaml_parser_parse(&parser, &event))
                        break;
                    params[param_idx].key = strdup((char *)event.data.scalar.value);
                }
                else if (strcmp((char *)event.data.scalar.value, "type") == 0)
                {
                    // Expect the next event to be the value of name
                    if (!yaml_parser_parse(&parser, &event))
                        break;
                    if (safe_atoi((char *)event.data.scalar.value, (int *)&params[param_idx].value_case) < 0)
                    {
                        return -1;
                    }
                }
                else if (strcmp((char *)event.data.scalar.value, "value") == 0)
                {
                    // Expect the next event to be the value of param_id
                    if (!yaml_parser_parse(&parser, &event))
                        break;
                    switch (params[param_idx].value_case)
                    {
                        case BOOL_VALUE:
                            if (strcmp((char *)event.data.scalar.value, "true") == 0)
                            {
                                params[param_idx].bool_value = 1;
                            }
                            else if (strcmp((char *)event.data.scalar.value, "false") == 0)
                            {
                                params[param_idx].bool_value = 0;
                            }
                            else
                            {
                                fprintf(stderr, "Error: Could not parse %s to boolean value\nAllowed values are: true, false\n", (char *)event.data.scalar.value);
                                return -1;
                            }
                            break;
                        case INT_VALUE:
                            if (safe_atoi((char *)event.data.scalar.value, (int *)&params[param_idx].int_value) < 0)
                            {
                                return -1;
                            }
                            break;
                        case FLOAT_VALUE:
                            if (safe_atof((char *)event.data.scalar.value, &params[param_idx].float_value) < 0)
                            {
                                return -1;
                            }
                            break;
                        case STRING_VALUE:
                            params[param_idx].string_value = strdup((char *)event.data.scalar.value);
                            break;
                        default:
                            fprintf(stderr, "Error: Value case %d unknown.\n", params[param_idx].value_case);
                            return -1;
                    }
                }
                else
                {
                    // Unexpected event type
                    printf("Error: Unexpected YAML scalar value format: %s\nAllowed values are: key, type, value\n", (char *)event.data.scalar.value);
                    return -1;
                }
            default:
                break;
        }
        if (event.type == YAML_SEQUENCE_END_EVENT || event.type == YAML_DOCUMENT_END_EVENT)
            break;

        yaml_event_delete(&event);
    }

    // Insert ModuleParameter into ModuleParameterList
    module_parameter_list->n_parameters = param_count;
    module_parameter_list->parameters = malloc(sizeof(ModuleParameter *) * param_count);

    if (module_parameter_list->parameters == NULL)
    {
        fprintf(stderr, "[yaml_parser] Error: Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }


    for (size_t i = 0; i < param_count; i++)
    {
        module_parameter_list->parameters[i] = &params[i];
    }

    cleanup_resources(&parser, &event, fh);

    return 0;
}