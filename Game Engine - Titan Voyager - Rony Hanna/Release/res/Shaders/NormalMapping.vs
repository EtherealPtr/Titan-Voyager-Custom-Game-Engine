#version 440 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_uv;
layout (location = 3) in vec3 vertex_normals;
layout (location = 4) in vec3 vertex_tangent;

out VS_OUT
{
	vec4 vertexColor; 
	vec2 vertexUv;
	vec3 fragPos;
	mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view *  model * vec4(vertex_position, 1.0f);
	vs_out.vertexColor = vec4(vertex_color, 1.0f);
	vs_out.vertexUv = vertex_uv;
	vs_out.fragPos = vec3(model * vec4(vertex_position, 1.0f));
	
	// Create TBN matrix
	vec3 T = normalize(vec3(model * vec4(vertex_tangent, 0.0f)));
	vec3 N = normalize(vec3(model * vec4(vertex_normals, 0.0f)));
	vec3 B = cross(N, T);
	
	vs_out.TBN = mat3(T, B, N);
}