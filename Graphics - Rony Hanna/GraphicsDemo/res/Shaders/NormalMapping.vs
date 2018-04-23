#version 430 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_uv;
layout (location = 3) in vec3 vertex_normals;
layout (location = 4) in vec3 vertex_tangent;

out vec4 vertexColor; 
out vec2 vertexUv;
out vec3 vertexNorms;
out vec3 fragPos;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view *  model * vec4(vertex_position, 1.0f);
	vertexColor = vec4(vertex_color, 1.0f);
	vertexUv = vertex_uv;
	fragPos = vec3(model * vec4(vertex_position, 1.0f));
	vertexNorms = mat3(transpose(inverse(model))) * vertex_normals;
	
	// Create TBN matrix
	vec3 T = normalize(vec3(model * vec4(vertex_tangent, 0.0f)));
	vec3 N = normalize(vec3(model * vec4(vertex_normals, 0.0f)));
	vec3 B = cross(N, T);
	
	TBN = mat3(T, B, N);
}