#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
	color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}
