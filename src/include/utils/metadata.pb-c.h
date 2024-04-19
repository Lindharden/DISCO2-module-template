/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: metadata.proto */

#ifndef PROTOBUF_C_metadata_2eproto__INCLUDED
#define PROTOBUF_C_metadata_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1005000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct MetadataItem MetadataItem;
typedef struct Metadata Metadata;


/* --- enums --- */


/* --- messages --- */

typedef enum {
  METADATA_ITEM__VALUE__NOT_SET = 0,
  METADATA_ITEM__VALUE_BOOL_VALUE = 2,
  METADATA_ITEM__VALUE_INT_VALUE = 3,
  METADATA_ITEM__VALUE_FLOAT_VALUE = 4,
  METADATA_ITEM__VALUE_STRING_VALUE = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(METADATA_ITEM__VALUE__CASE)
} MetadataItem__ValueCase;

/*
 * Custom metadata item as a key-value pair
 */
struct  MetadataItem
{
  ProtobufCMessage base;
  char *key;
  MetadataItem__ValueCase value_case;
  union {
    protobuf_c_boolean bool_value;
    int32_t int_value;
    float float_value;
    char *string_value;
  };
};
#define METADATA_ITEM__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&metadata_item__descriptor) \
    , (char *)protobuf_c_empty_string, METADATA_ITEM__VALUE__NOT_SET, {0} }


/*
 * Image metadata definition
 */
struct  Metadata
{
  ProtobufCMessage base;
  int32_t size;
  int32_t height;
  int32_t width;
  int32_t channels;
  int32_t timestamp;
  int32_t bits_pixel;
  int32_t image_offset;
  char *camera;
  size_t n_items;
  MetadataItem **items;
};
#define METADATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&metadata__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0, (char *)protobuf_c_empty_string, 0,NULL }


/* MetadataItem methods */
void   metadata_item__init
                     (MetadataItem         *message);
size_t metadata_item__get_packed_size
                     (const MetadataItem   *message);
size_t metadata_item__pack
                     (const MetadataItem   *message,
                      uint8_t             *out);
size_t metadata_item__pack_to_buffer
                     (const MetadataItem   *message,
                      ProtobufCBuffer     *buffer);
MetadataItem *
       metadata_item__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   metadata_item__free_unpacked
                     (MetadataItem *message,
                      ProtobufCAllocator *allocator);
/* Metadata methods */
void   metadata__init
                     (Metadata         *message);
size_t metadata__get_packed_size
                     (const Metadata   *message);
size_t metadata__pack
                     (const Metadata   *message,
                      uint8_t             *out);
size_t metadata__pack_to_buffer
                     (const Metadata   *message,
                      ProtobufCBuffer     *buffer);
Metadata *
       metadata__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   metadata__free_unpacked
                     (Metadata *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*MetadataItem_Closure)
                 (const MetadataItem *message,
                  void *closure_data);
typedef void (*Metadata_Closure)
                 (const Metadata *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor metadata_item__descriptor;
extern const ProtobufCMessageDescriptor metadata__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_metadata_2eproto__INCLUDED */