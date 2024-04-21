project('ml', 'c', 'cpp')
project_name = 'ml'

# Compiler and linker flags
cflags = ['-Wall']

# External dependencies
m_dep = meson.get_compiler('c').find_library('m', required : false)
proto_c_dep = meson.get_compiler('c').find_library('libprotobuf-c', required : true)
tf_lite_dep = meson.get_compiler('c').find_library('tensorflow-lite', required : true)
opencv_dep = meson.get_compiler('c').find_library('opencv', required : true)

# Source files
sources = [
    'src/module_template.cpp', # module file (REMEMBER TO CHANGE!)
    'src/utils/memory_util.c',
    'src/utils/config_util.c',
    'src/utils/batch_util.c',
    'src/utils/error_util.c',
    'src/utils/metadata_util.c',
    'src/utils/metadata.pb-c.c',
]

dirs = include_directories(
    'src/include', 
    'src/include/utils',
)

# Shared library (SO)
shared_library(project_name, sources,
    include_directories: dirs,
    c_args: cflags + ['-DSHARED_MEMORY=1'],
    dependencies: [proto_c_dep, tf_lite_dep, opencv_dep]
)

# Check whether we are cross-compiling or not
if not get_option('cross-compile')
    # If not cross-compiling, include the executable target
    sources += 'src/test.c'
    sources += 'src/utils/yaml_parser.c'

    libyaml_dep = dependency('yaml-0.1')

    # Executable
    executable(project_name + '-exec', sources,
        include_directories: dirs,
        c_args: cflags + ['-g', '-DSHARED_MEMORY=0'],
        dependencies: [libyaml_dep, m_dep, proto_c_dep, tf_lite_dep, opencv_dep]
    )
endif
