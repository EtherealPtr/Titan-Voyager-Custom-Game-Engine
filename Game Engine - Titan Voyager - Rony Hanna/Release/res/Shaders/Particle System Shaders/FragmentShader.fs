#version 440 core
out vec4 FragColor;
in vec2 vertex_uv;

uniform sampler2D textureImg;

void main()
{
	vec4 color = texture(textureImg, vertex_uv);
	if (color.r <= 0.1f && color.g <= 0.1f && color.b <= 0.1f) discard;
	
    FragColor = color;
} 