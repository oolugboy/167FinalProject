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
	else if(isTexture == 2){
		vec4 sum = vec4(0);
		vec2 texcoord = vec2(TexCoord[0]);
		int j;
		int i;
		for( i= -4 ;i < 4; i++){
			for (j = -3; j < 3; j++){
				sum += texture2D(tile, texcoord + vec2(j, i)*0.004) * 0.25;
			}
		}
		if (texture2D(tile, texcoord).r < 0.3) {
			color = sum*sum*0.012 + texture2D(tile, texcoord);
		}
		else{
			if (texture2D(tile, texcoord).r < 0.5)
			{
				color = sum*sum*0.009 + texture2D(tile, texcoord);
			}
			else
			{
				color = sum*sum*0.0075 + texture2D(tile, texcoord);
			}
		}
	}
	else{
		color = vec4(0.5f, 0.5f, 0.5f, sampleExtraOutput);
	}
}