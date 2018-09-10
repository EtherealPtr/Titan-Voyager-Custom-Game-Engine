#version 440 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 VertexUv;

uniform sampler2D sampler;

void main()
{
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 color = texture(sampler, VertexUv);
	
	if (color.a <= 0.5f)
		discard;
	
	FragColor = color;
} 