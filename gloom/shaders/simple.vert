#version 430 core

in vec3 position;
in vec4 color;

out vec4 colorIn;

uniform layout(position = 0) mat4 transfMat;

void main()
{
    gl_Position = transfMat * vec4(position, 1.0f);
    colorIn = color;
}