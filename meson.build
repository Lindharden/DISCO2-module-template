project('example', 'c', 'cpp')
project_name = 'example'

# Compiler and linker flags
cflags = ['-Wall']

# External dependencies
m_dep = meson.get_compiler('c').find_library('m', required : false)
libyaml_dep = dependency('yaml-0.1')

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
    'lib/tensorflow',
    'lib/opencv',
)

# Shared library (SO)
shared_library(project_name, sources,
    include_directories: dirs,
    c_args: cflags + ['-DSHARED_MEMORY=1'],
    dependencies: []
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
        dependencies: [libyaml_dep, m_dep]
    )
endif
