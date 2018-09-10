#version 440 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_uv;

out VS_OUT 
{
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_out.TexCoords = vertex_uv;
	gl_Position = projection * view *  model * vec4(vertex_position, 1.0f);
}