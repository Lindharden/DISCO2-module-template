# DISCO2-module-template

Follow this guide for developing a module for the DISCO2 Image Processing Pipeline

## Prerequisites

Install the following packages:
```
sudo apt install build-essential libyaml-dev gcc meson ninja pkg-config gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
```

Rename the project inside the `meson.build` file, by editing the `project_name` variable and the project name. This name is used for the shared library files, and the test executable.

## Writing the Module

To write the module, you must edit the `module_template.c` file. This file gives examples of how the `ImageBatch` struct should be utilized, and how parameters can be accessed. 

The `ImageBatch` struct looks as follows:

```c
typedef struct ImageBatch {
    long mtype;
    int height;
    int width;
    int channels;
    int num_images;
    int shm_key;
    int pipeline_id;
    unsigned char *data;
} ImageBatch;
```

The `ImageBatch` struct contains metadata about the batch of images it contains (such as height, width and channels), and about the shared memory space and which pipeline should be utilized (not important for development). The image data from all the individual images in the batch are stored sequentially in the `data` field. Because of this it will be necessary to iterate over multiple images in the `data` field using the `num_images` field as an indicator for how many images are present in the data.

When the image batch has been processed, the `finalize` function must be called, in order to copy the new image data to memory. The `finalize` function can be called like this:
```c
finalize(&result_batch, input_batch);
```

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

The modules support external dependencies. These will be compiled with the shared object library itself. To add external dependencies, you must add them in the `meson.build` file, just like the already existing dependencies:

```meson
proto_c_dep = dependency('libprotobuf-c', fallback: ['protobuf-c', 'proto_c_dep'])
m_dep = meson.get_compiler('c').find_library('m', required : false)
libyaml_dep = dependency('yaml-0.1')   
...       
deps = [m_dep, proto_c_dep, ...]
```

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
