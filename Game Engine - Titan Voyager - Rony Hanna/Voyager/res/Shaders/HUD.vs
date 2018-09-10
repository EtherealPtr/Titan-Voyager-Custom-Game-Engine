#version 440 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_uv;

out vec4 vertexColor; 
out vec2 VertexUv; 

uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * model * vec4(vertex_position, 1.0f);
	vertexColor = vec4(vertex_color, 1.0f);
	VertexUv = vertex_uv;
}