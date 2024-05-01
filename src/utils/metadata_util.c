#include "util.h"

MetadataList *metadata;

void unpack_metadata()
{
    metadata = malloc(sizeof(MetadataList *));
    if (metadata == NULL)
    {
        signal_error_and_exit(100);
    }
    metadata->n_metadata = input->num_images;
    metadata->metadata = malloc(input->num_images * sizeof(Metadata *));
    if (metadata->metadata == NULL)
    {
        signal_error_and_exit(100);
    }

    uint32_t offset = 0;
    int image_index = 0;

    while (image_index < input->num_images && offset < input->batch_size)
    {
        uint32_t metadata_size = *((uint32_t *)(input->data + offset));
        offset += sizeof(uint32_t); // Move the offset to the start of the metadata

        Metadata *meta = metadata__unpack(NULL, metadata_size, input->data + offset);
        offset += metadata_size;

        metadata->metadata[image_index] = malloc(sizeof(Metadata));
        if (metadata->metadata[image_index] == NULL)
        {
            signal_error_and_exit(100);
        }
        metadata->metadata[image_index]->size = meta->size;
        metadata->metadata[image_index]->height = meta->height;
        metadata->metadata[image_index]->width = meta->width;
        metadata->metadata[image_index]->channels = meta->channels;
        metadata->metadata[image_index]->timestamp = meta->timestamp;
        metadata->metadata[image_index]->bits_pixel = meta->bits_pixel;
        metadata->metadata[image_index]->image_offset = offset;
        metadata->metadata[image_index]->camera = strdup(meta->camera);
        metadata->metadata[image_index]->n_items = meta->n_items;
        metadata->metadata[image_index]->items = malloc(meta->n_items * sizeof(MetadataItem *));
        if (metadata->metadata[image_index]->items == NULL)
        {
            signal_error_and_exit(100);
        }

        for (size_t i = 0; i < meta->n_items; i++)
        {
            metadata->metadata[image_index]->items[i] = malloc(sizeof(MetadataItem));
            if (metadata->metadata[image_index]->items[i] == NULL)
            {
                signal_error_and_exit(100);
            }
            metadata->metadata[image_index]->items[i]->key = strdup(meta->items[i]->key);
            metadata->metadata[image_index]->items[i]->value_case = meta->items[i]->value_case;

            switch (meta->items[i]->value_case)
            {
            case METADATA_ITEM__VALUE_BOOL_VALUE:
                metadata->metadata[image_index]->items[i]->bool_value = meta->items[i]->bool_value;
                break;
            case METADATA_ITEM__VALUE_INT_VALUE:
                metadata->metadata[image_index]->items[i]->int_value = meta->items[i]->int_value;
                break;
            case METADATA_ITEM__VALUE_FLOAT_VALUE:
                metadata->metadata[image_index]->items[i]->float_value = meta->items[i]->float_value;
                break;
            case METADATA_ITEM__VALUE_STRING_VALUE:
                metadata->metadata[image_index]->items[i]->string_value = strdup(meta->items[i]->string_value);
                break;
            default:
                break;
            }
        }

        metadata__free_unpacked(meta, NULL);

        offset += meta->size; // Move the offset to the start of the next image block

        image_index++;
    }
}

static MetadataItem *get_item(Metadata *data, const char *key) {
    MetadataItem *found_item = NULL;
    for (size_t i = 0; i < data->n_items; i++)
    {
        if (strcmp(data->items[i]->key, key) == 0)
        {
            found_item = data->items[i];
            break;
        }
    }
    return found_item;
}

static MetadataItem *allocate_metadata_item(Metadata *data, char *key)
{
    if (get_item(data, key) != NULL)
    {
        signal_error_and_exit(511);
    }
    if (data->n_items == 0)
    {
        data->items = malloc(sizeof(MetadataItem *));
        if (data->items == NULL)
        {
            signal_error_and_exit(100);
        }
    }
    else
    {
        MetadataItem **tmp = realloc(data->items, (data->n_items + 1) * sizeof(MetadataItem *));
        if (tmp == NULL)
        {
            signal_error_and_exit(101);
        }
        data->items = tmp;
    }
    data->items[data->n_items] = malloc(sizeof(MetadataItem));
    if (data->items[data->n_items] == NULL)
    {
        signal_error_and_exit(100);
    }
    MetadataItem item = METADATA_ITEM__INIT;
    *data->items[data->n_items] = item;
    data->items[data->n_items]->key = strdup(key);
    return data->items[data->n_items++];
}

void add_custom_metadata_bool(Metadata *data, char *key, int val)
{
    MetadataItem *item = allocate_metadata_item(data, key);
    item->value_case = METADATA_ITEM__VALUE_BOOL_VALUE;
    item->bool_value = val;
}

void add_custom_metadata_int(Metadata *data, char *key, int val)
{
    MetadataItem *item = allocate_metadata_item(data, key);
    item->value_case = METADATA_ITEM__VALUE_INT_VALUE;
    item->int_value = val;
}

void add_custom_metadata_float(Metadata *data, char *key, float val)
{
    MetadataItem *item = allocate_metadata_item(data, key);
    item->value_case = METADATA_ITEM__VALUE_FLOAT_VALUE;
    item->float_value = val;
}

void add_custom_metadata_string(Metadata *data, char *key, char *val)
{
    MetadataItem *item = allocate_metadata_item(data, key);
    item->value_case = METADATA_ITEM__VALUE_STRING_VALUE;
    item->string_value = strdup(val);
}

int get_custom_metadata_bool(Metadata *data, char *key)
{
    MetadataItem *found_item = get_item(data, key);

    if (found_item == NULL)
    {
        signal_error_and_exit(507);
    }
    
    return found_item->bool_value;
}

int get_custom_metadata_int(Metadata *data, char *key)
{
    MetadataItem *found_item = get_item(data, key);

    if (found_item == NULL)
    {
        signal_error_and_exit(508);
    }
    
    return found_item->int_value;
}

float get_custom_metadata_float(Metadata *data, char *key)
{
    MetadataItem *found_item = get_item(data, key);

    if (found_item == NULL)
    {
        signal_error_and_exit(509);
    }
    
    return found_item->float_value;
}

char *get_custom_metadata_string(Metadata *data, char *key)
{
    MetadataItem *found_item = get_item(data, key);

    if (found_item == NULL)
    {
        signal_error_and_exit(510);
    }
    
    return found_item->string_value;
}

Metadata *get_metadata(int index)
{
    if (index >= metadata->n_metadata)
        return NULL;
    return metadata->metadata[index];
}