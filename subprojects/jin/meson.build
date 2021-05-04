project('jin', 'cpp',
    version: '0.1 WIP'
)

include_dirs = include_directories('include')
sources = []
sources += 'src/Application.cpp'
sources += 'src/IO.cpp'
sources += 'src/Layer.cpp'
sources += 'src/Memory.cpp'
sources += 'src/Renderer.cpp'
sources += 'src/Vector2.cpp'
sources += 'src/Vector3.cpp'
sources += 'src/Vector3.cpp'
sources += 'src/Matrix4.cpp'
sources += 'src/Window.cpp'
sources += 'src/Shader.cpp'
sources += 'src/ShaderProgram.cpp'
sources += 'src/VertexArrayObject.cpp'
sources += 'src/VertexBufferObject.cpp'
sources += 'src/IndexBufferObject.cpp'

glew_dep = dependency('glew')
glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')
imgui = subproject('imgui')
imgui_dep = imgui.get_variable('imgui_dep')

jin = library('jin',
    sources,
    dependencies: [glfw_dep, glew_dep, gl_dep, imgui_dep],
    include_directories: include_dirs,
)

jin_dep = declare_dependency(include_directories: include_dirs, link_with: jin)