#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in float texture_id;
layout(location = 4) in float tilingFactor;

out vec4 v_color;
out vec2 v_uv;
out float v_tilingFactor;
out float v_texture_id;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    v_color = color;
    v_uv = uv;
    v_texture_id = texture_id;
    v_tilingFactor = tilingFactor;
    gl_Position = proj * view * model * vec4(pos.x, pos.y, 0, 1);
}