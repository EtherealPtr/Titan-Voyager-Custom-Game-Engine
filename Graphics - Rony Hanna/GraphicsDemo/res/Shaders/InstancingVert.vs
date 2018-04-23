#version 430 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normals;
layout (location = 2) in vec2 vertex_uv;
layout (location = 3) in mat4 instanceMatrix;

out vec2 vertexUv;
out vec3 vertexNormals;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view *  instanceMatrix * vec4(vertex_position, 1.0f);
	vertexUv = vertex_uv;
	vertexNormals = vertex_normals;
}