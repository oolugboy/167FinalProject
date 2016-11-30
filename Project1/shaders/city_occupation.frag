#version 330 core
// This is a sample fragment shader.

uniform int code;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    color = vec4(code/255.0, 0.0f, 0.0f, 0.0f);
}