# DISCO2-module-template

Follow this guide for developing a module for the DISCO2 Image Processing Pipeline.

Start by creating a new repository from this template by clicking the "Use this template" button at the top-right of the GitHub page.

## Prerequisites

Install the following packages:
```
sudo apt install build-essential libyaml-dev gcc meson ninja pkg-config gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu
```

Rename the project inside the `meson.build` file, by editing the `project_name` variable and the project name. This name is used for the shared library files, and the test executable.

## Writing the Module

To write the module, you must edit the `module_template.c` file. This file gives examples of how to utilize the `ImageBatch` struct and how parameters can be accessed. 

### ImageBatch Struct and Utilities

The `ImageBatch` struct looks as follows:

```c
typedef struct ImageBatch {
    long mtype;
    int height;
    int width;
    int channels;
    int num_images;
    int batch_size;
    int shm_key;
    int pipeline_id;
    unsigned char *data;
} ImageBatch;
```

The `ImageBatch` struct serves as a container for a collection of images, containing metadata such as their dimensions (height, width) and color channels. It also holds information regarding shared memory allocation and the specific pipeline that should be employed for processing (not important for development). The struct's data field sequentially stores the image data, with each individual image prefaced by its size—a numerical prefix detailing the byte count of the image's data. This organization facilitates the delineation and access of images within a continuous memory space.

### Important Consideration for Input and Result Batches
It's imperative to note that the input ImageBatch should remain unaltered and modifications are exclusively reserved for the result batch, facilitated through the provided utility functions.

### Retrieving Images

To access specific images within the batch, the `get_image_data` function can be used:
 
- `get_image_data(int index, unsigned char **out)`: Retrieves the data of an image at the specified index, allocating memory and returning the size of the image data.

Developers can iterate through the batch by incrementally advancing an index, from 0 up to the total number of images in the back (num_images - 1), using `get_image_data` at each step.


### Utility Functions

Several utility functions is implemented to faciliate interaction and manipulation of both the input and resulting image batch, encapsulated within `batch_util.c`. These functions are divided into two distinct categories: getters for querying properties of the input ImageBatch and setters/manipulation functions for altering the state of the resulting image batch (result). 

#### Input Batch Utilities

The utility functions for the input ImageBatch include:

- `get_input_width()`
- `get_input_height()`
- `get_input_channels()`
- `get_input_num_images()`
- `get_image_data(int index, unsigned char **out)`: Retrieves the data of an image at the specified index, allocating memory and returning the size of the image data.

#### Result Batch Utilities

For modifying the result batch, the utilities provide:

- `set_result_width(int width)`
- `set_result_height(int height)`
- `set_result_channels(int channels)`
- `set_result_dimensions(int width, int height, int channels)`: Sets the width, height, and number of channels of the result batch.
- `set_result_num_images(int num_images)`: Adjusts the total number of images in the result batch.
- `append_result_image(unsigned char *data, size_t data_size)`: Appends an image to the batch, including size prefixing.



## Providing Custom Parameters

If your module takes some custom parameters, they can be defined in the `config.yaml` file, following the syntax:
```yaml
- key: param_name_1
  type: 2
  value: true

- key: param_name_2
  type: 3
  value: 256

- key: param_name_3
  type: 4
  value: 3.1415

- key: param_name_4
  type: 5
  value: DISCO
```

The types can be translated to logical types, following this table:

| Type ID  | Logical Type |
| -------- | ------------ |
| 2        | Boolean      |
| 3        | int          |
| 4        | float        |
| 5        | string       |

The parameters from the `config.yaml` can be accessed within the code like this:

```c
int param_1 = get_param_bool(config, "param_name_1");
int param_2 = get_param_int(config, "param_name_2");
float param_3 = get_param_float(config, "param_name_3");
char *param_4 = get_param_string(config, "param_name_4");
```

## Adding External Dependencies

**Modules are required to be fully self-contained, which entails that any external dependencies must be statically compiled into the module.**

To add external dependencies, you must add them in the `meson.build` file (create dependencies and add them to the given compile targets), just like in this example:
```meson
m_dep = meson.get_compiler('c').find_library('m', required : false)
libwebp_dep = dependency('libwebp', fallback: ['webp'], static: true)
...       
dependencies: [libwebp_dep]
```

Keep in mind that the external dependencies must be compiled to the given architecture that you are compiling your module towards and be compiled statically to ensure the module is fully self-contained.

## Building the Module

To build the module, run the `configure` script with either `build` or `test` as argument, as following (using build):
```sh
./configure build
```

Use `build` when you want to cross-compile to AArch64, and use `test` if you want to compile to your current machine for testing purposes.

Compiling with the `test` argument will produce the following files in the `builddir` directory:
 - `*project_name*-exec`       | Can be executed to test the module (Will not utilize shared memory for the sake of simplicity).
 - `*project_name*.so`         | Shared Oject library for use on the host architecture.

Compiling with the `build` argument will produce the following files in the `builddir` directory:
 - `*project_name*.so`         | Shared Oject library for use on AArch64 machines.

## Testing the Module

In order to test and debug the module, you must first compile the module as previously described. This will produce the executable `*project_name*-exec` located in the `builddir` directory. This executable is able to be run using a debugger, but must be called from the workspace root. For `launch.json` in vscode, the program field should have value:
```json
"program": "${workspaceFolder}/builddir/example-exec"
```

If the module expects custom parameters, these must be specified in the `config.yaml` file as explained in the [Providing Custom Parameters](#providing-custom-parameters) section.
