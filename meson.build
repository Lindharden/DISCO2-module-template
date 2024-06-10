project('classify', 'c', 'cpp')
project_name = 'classify'

# Compiler and linker flags
cflags = ['-Wall']

# Source files
sources = [
    'src/classify.cpp', # module file (REMEMBER TO CHANGE!)
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
    'usr/include',
    # 'usr/include/opencv4'
)

# For initial testing
# https://github.com/karthickai/tflite/blob/master/05_TFLite_Image_Classification/main.cpp
proto_c_dep = meson.get_compiler('c').find_library('protobuf-c', required : false)
opencv_dep = dependency('opencv4')

# TODO: Compile tensorflowlite for Aarch64
# libs_you_need_to_link = ['protobuf-c', 'opencv_core', 'opencv_imgproc']
libs_you_need_to_link = ['tensorflowlite']
base_path = join_paths(meson.current_source_dir(), 'usr')

deps = []
foreach lib_name : libs_you_need_to_link 
    deps += meson.get_compiler('c').find_library(lib_name, dirs : [join_paths(base_path, 'lib')], )
endforeach

# For initial testing
deps += [proto_c_dep, opencv_dep]

# Shared library (SO)
shared_library(project_name, sources,
    include_directories: dirs,
    c_args: cflags + ['-DSHARED_MEMORY=1'],
    dependencies: deps
)

# Check whether we are cross-compiling or not
if not get_option('cross-compile')
    # If not cross-compiling, include the executable target
    sources += 'src/test.c'
    sources += 'src/utils/yaml_parser.c'

    m_dep = meson.get_compiler('c').find_library('m', required : false)
    libyaml_dep = dependency('yaml-0.1')

    deps += m_dep
    deps += libyaml_dep

    # Executable
    executable(project_name + '-exec', sources,
        include_directories: dirs,
        c_args: cflags + ['-g', '-DSHARED_MEMORY=0'],
        dependencies: deps
    )
endif