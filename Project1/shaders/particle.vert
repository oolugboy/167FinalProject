#version 330 core
layout (location = 0) in vec3 vertex; // <vec2 position, vec2 texCoords>


out vec3 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 10.0f;
    TexCoords = vertex.zw;
    ParticleColor = color;
    gl_Position = projection * * view * vec4(vec3((vertex.x, 0.0, vertex.z) * scale + vec3(offset.x, 0.0, offset.y), 1.0);
}