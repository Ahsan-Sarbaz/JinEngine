project('imgui', 'cpp',
    version: '1.83 WIP Docking',
    license: 'MIT',
)

include_dirs = include_directories('.', 'backends')
sources = ['imgui_demo.cpp', 'imgui_draw.cpp', 'imgui_tables.cpp', 'imgui_widgets.cpp', 'imgui.cpp']
sources += 'backends/imgui_impl_opengl3.cpp'
sources += 'backends/imgui_impl_glfw.cpp'

cpp = meson.get_compiler('cpp')
dependencies = []

glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')

dependencies += glfw_dep
dependencies += glew_dep

imgui = library('imgui',
    sources,
    dependencies: dependencies,
    include_directories: include_dirs,
)

imgui_dep = declare_dependency(include_directories: include_dirs, link_with: imgui)
