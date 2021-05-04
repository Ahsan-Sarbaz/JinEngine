#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 color;

out vec4 v_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    v_color = color;
    gl_Position = proj * view * model * vec4(pos.x, pos.y, 0, 1);
}