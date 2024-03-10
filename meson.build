project('example', 'c', subproject_dir: 'lib',)

# Compiler and linker flags
cflags = ['-Wall']

proto_c_dep = dependency('libprotobuf-c', fallback: ['protobuf-c', 'proto_c_dep'])
m_dep = meson.get_compiler('c').find_library('m', required : false)
libyaml_dep = dependency('yaml-0.1')
          
deps = [m_dep, proto_c_dep]

# Source files
sources = [
    'src/module.c',
    'src/utils/module_config.pb-c.c',
    'src/utils/memory_util.c',
    'src/utils/config_util.c',
]

# Shared library
shared_library('example', sources,
    include_directories: include_directories('src/include', 'src/include/utils'),
    c_args: cflags + ['-DSHARED_MEMORY=1'],
    dependencies: deps
)

sources += 'src/test.c'
deps += libyaml_dep

# Executable
executable('executable', sources,
    include_directories: include_directories('src/include', 'src/include/utils'),
    c_args: cflags + ['-DSHARED_MEMORY=0'],
    dependencies: deps
)
