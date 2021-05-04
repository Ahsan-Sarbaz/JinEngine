project('sokuban', 'cpp',
    version: '0.1',
    default_options: ['cpp_std=c++17', 'cpp_args=-O3'])

jin = subproject('jin')
jin_dep = jin.get_variable('jin_dep')

imgui = subproject('imgui')
imgui_dep = imgui.get_variable('imgui_dep')

sokubak_src = []
sokubak_src += 'src/main.cpp'
sokubak_src += 'src/MainLayer.cpp'
sokubak_src += 'src/ImGUILayer.cpp'

sokuban_inc_dir = include_directories(['include'])

executable('sokuban', sokubak_src, include_directories: sokuban_inc_dir, dependencies: [imgui_dep, jin_dep])