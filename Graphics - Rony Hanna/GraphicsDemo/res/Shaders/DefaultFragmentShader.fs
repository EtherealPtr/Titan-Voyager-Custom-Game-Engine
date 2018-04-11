#version 430 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 vertexUv;

uniform sampler2D meshTexture;

void main()
{
	vec4 texColor = texture(meshTexture, vertexUv);
	if (texColor.a < 0.1f)
		discard;
    FragColor = texColor;
} 