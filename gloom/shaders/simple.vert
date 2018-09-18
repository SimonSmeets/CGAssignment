#version 430 core

in vec3 position;
in vec4 color;

out vec4 colorIn;


void main()
{
    gl_Position = vec4(position, 1.0f);
    colorIn = color;
}