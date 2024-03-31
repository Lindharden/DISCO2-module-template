project('example', 'c', 'cpp')
project_name = 'example'

# Compiler and linker flags
cflags = ['-Wall']

# External dependencies
m_dep = meson.get_compiler('c').find_library('m', required : false)
opencv_dep = dependency('opencv4')
libyaml_dep = dependency('yaml-0.1')

# tflite dependency
# build c library like this: https://www.tensorflow.org/lite/guide/build_cmake#build_tensorflow_lite_c_library
tflite_dep = declare_dependency(
    include_directories: include_directories('lib/tensorflow'),
    link_with: files('lib/tensorflow/tflite_build/libtensorflowlite_c.so')
)

# Source files
sources = [
    'src/module_template.cpp', # module file (REMEMBER TO CHANGE!)
    'src/utils/memory_util.c',
    'src/utils/config_util.c',
    'src/utils/batch_util.c',
    'src/utils/error_util.c',
]

dirs = include_directories(
    'src/include', 
    'src/include/utils',
    'lib/libedgetpu/tflite/public',
)

# Shared library (SO)
shared_library(project_name, sources,
    include_directories: dirs,
    c_args: cflags + ['-DSHARED_MEMORY=1'],
    dependencies: [opencv_dep, tflite_dep]
)

# Check whether we are cross-compiling or not
if not get_option('cross-compile')
    # If not cross-compiling, include the executable target
    sources += 'src/test.c'
    sources += 'src/utils/yaml_parser.c'

    # Executable
    executable(project_name + '-exec', sources,
        include_directories: dirs,
        c_args: cflags + ['-g', '-DSHARED_MEMORY=0'],
        dependencies: [libyaml_dep, m_dep, opencv_dep, tflite_dep]
    )
endif
