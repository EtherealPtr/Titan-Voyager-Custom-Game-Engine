#version 430 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_uv;
layout (location = 3) in mat4 instanceMat;

out vec4 vertexColor; 
out vec2 vertexUv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * instanceMat * vec4(vertex_position, 1.0f);
	vertexColor = vec4(vertex_color, 1.0f);
	vertexUv = vertex_uv;
}