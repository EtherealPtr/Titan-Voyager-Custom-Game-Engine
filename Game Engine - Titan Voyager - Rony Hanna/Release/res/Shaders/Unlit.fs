#version 440 core
out vec4 FragColor;

in VS_OUT 
{
    vec2 TexCoords;
} fs_in;

uniform sampler2D meshTexture;

void main()
{
	vec4 texColor = texture(meshTexture, fs_in.TexCoords);
    FragColor = texColor;
} 