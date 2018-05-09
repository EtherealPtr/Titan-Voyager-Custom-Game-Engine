#version 430 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(vertex_position, 1.0f);
}