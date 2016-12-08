#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 Normal;
in vec3 Position;
out vec4 color;

uniform vec3 cam_pos;
uniform samplerCube texBox;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
	vec3 I = normalize(Position - cam_pos);
    vec3 R = reflect(I, normalize(Normal));
    color = texture(texBox, R);
}