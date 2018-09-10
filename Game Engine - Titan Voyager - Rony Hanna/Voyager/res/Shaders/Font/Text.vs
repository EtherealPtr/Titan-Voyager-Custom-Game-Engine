#version 440 core

in layout (location = 0) vec4 vertex_position;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(vertex_position.xy, 0.0f, 1.0f);
	TexCoords = vertex_position.zw;
}