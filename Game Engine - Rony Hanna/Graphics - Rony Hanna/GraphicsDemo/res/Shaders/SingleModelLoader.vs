#version 430 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_Uv;

out vec2 TexCoords;
out vec3 vertexNormal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = vertex_Uv;    
    gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
	vertexNormal = vertex_normal;
	fragPos = vec3(model * vec4(vertex_position, 1.0f));
}