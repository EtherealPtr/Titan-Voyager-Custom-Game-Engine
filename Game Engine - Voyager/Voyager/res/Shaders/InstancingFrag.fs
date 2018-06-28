#version 430 core

out vec4 FragColor;
in vec2 vertexUv;
in vec3 vertexNormals;

uniform sampler2D meshTexture;

void main()
{
	vec4 texColor = texture(meshTexture, vertexUv);
    FragColor = texColor;
} 