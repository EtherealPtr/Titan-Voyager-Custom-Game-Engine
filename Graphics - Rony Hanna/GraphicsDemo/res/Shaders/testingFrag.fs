#version 430 core

out vec4 FragColor;
in vec2 vertexUv;

uniform sampler2D meshTexture;

void main()
{
	vec4 texColor = texture(meshTexture, vertexUv);
    FragColor = texColor;
} 