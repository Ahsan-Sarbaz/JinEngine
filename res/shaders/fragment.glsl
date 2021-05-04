#version 330 core

out vec4 final_color;

in vec4 v_color;
void main()
{
    final_color = v_color;
}