#version 430 core

out vec4 FragColor;
out vec2 vertex_uv;

uniform sampler2D textureImg;
uniform vec4 vertexColor;

void main()
{
    FragColor = vertexColor; //texture(textureImg, vertex_uv);
} 