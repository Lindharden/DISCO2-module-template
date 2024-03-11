project('example', 'c')
project_name = 'example'

# Compiler and linker flags
cflags = ['-Wall']

# External dependencies
proto_c_dep = dependency('libprotobuf-c', fallback: ['protobuf-c', 'proto_c_dep'])
m_dep = meson.get_compiler('c').find_library('m', required : false)
libyaml_dep = dependency('yaml-0.1')          
deps = [m_dep, proto_c_dep]

# Source files
sources = [
    'src/module_template.c',
    'src/utils/module_config.pb-c.c',
    'src/utils/memory_util.c',
    'src/utils/config_util.c',
]

dirs = include_directories(
    'src/include', 
    'src/include/utils',
)

# Shared library (SO)
shared_library(project_name, sources,
    include_directories: dirs,
    c_args: cflags + ['-DSHARED_MEMORY=1'],
    dependencies: deps
)

sources += 'src/test.c'
sources += 'src/utils/yaml_parser.c'
deps += libyaml_dep

# Executable
executable(project_name + '-exec', sources,
    include_directories: dirs,
    c_args: cflags + ['-g', '-DSHARED_MEMORY=0'],
    dependencies: deps
)
