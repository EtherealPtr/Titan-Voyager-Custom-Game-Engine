#version 440 core
layout (location = 0) in vec3 vertex_position;

out vec3 skyboxTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view *  model * vec4(vertex_position, 1.0f);
	skyboxTexCoord = vertex_position;
}