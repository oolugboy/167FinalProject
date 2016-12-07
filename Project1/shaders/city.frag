#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec2 TexCoord;

uniform int isTexture;
uniform sampler2D tile;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    //color = vec4(0.5f, 1.0f, 0.0f, sampleExtraOutput);
	if(isTexture == 1){
		color = texture(tile, TexCoord);
	}
	else{
		color = vec4(0.0f, 0.0f, 1.0f, sampleExtraOutput);
	}
}