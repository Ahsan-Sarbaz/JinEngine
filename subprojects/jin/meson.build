project('jin', 'cpp',
    version: '0.2 WIP',
    default_options: ['cpp_std=c++20']
)

include_dirs = include_directories('include')
sources = []
sources += 'src/Application.cpp'
sources += 'src/IO.cpp'
sources += 'src/Layer.cpp'
sources += 'src/Renderer.cpp'
sources += 'src/Rect.cpp'
sources += 'src/Window.cpp'
sources += 'src/Shader.cpp'
sources += 'src/ShaderProgram.cpp'
sources += 'src/VertexArrayObject.cpp'
sources += 'src/VertexBufferObject.cpp'
sources += 'src/IndexBufferObject.cpp'
sources += 'src/stb_image.cpp'
sources += 'src/Texture.cpp'
sources += 'src/SpriteSheet.cpp'
sources += 'src/SpriteSheetAnimation.cpp'
sources += 'src/Time.cpp'
sources += 'src/Entity.cpp'
sources += 'src/Mesh.cpp'
sources += 'src/Model.cpp'
sources += 'src/EditorCamera.cpp'
sources += 'src/UniformBufferObject.cpp'
sources += 'src/CubeMap.cpp'
sources += 'src/Material.cpp'
sources += 'src/Level.cpp'
sources += 'src/Framebuffer.cpp'

glew_dep = dependency('glew')
glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')
assimp_dep = dependency('assimp')
imgui = subproject('imgui')
imgui_dep = imgui.get_variable('imgui_dep')

jin = static_library('jin',
    sources,
    dependencies: [glfw_dep, glew_dep, gl_dep, assimp_dep, imgui_dep],
    include_directories: include_dirs,
)

jin_dep = declare_dependency(include_directories: include_dirs, link_with: jin)
