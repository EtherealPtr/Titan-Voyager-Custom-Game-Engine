#version 430 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 VertexUv;

uniform sampler2D crossHair;

void main()
{
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 color = texture(crossHair, VertexUv);
	
	if (color.a <= 0.5f)
		discard;
	
	FragColor = color;
} 