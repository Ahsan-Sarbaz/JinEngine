project('JinEditor', 'cpp',
    version: '0.1',
    default_options: ['cpp_std=c++20']
)

jin = subproject('jin')
jin_dep = jin.get_variable('jin_dep')

imgui = subproject('imgui')
imgui_dep = imgui.get_variable('imgui_dep')

assimp_dep = dependency('assimp')

jin_editor_src = []
jin_editor_src += 'src/main.cpp'
jin_editor_src += 'src/MainLayer.cpp'

jin_editor_inc_dir = include_directories(['include'])

executable('JinEditor', jin_editor_src, include_directories: jin_editor_inc_dir, dependencies: [imgui_dep, jin_dep, assimp_dep])