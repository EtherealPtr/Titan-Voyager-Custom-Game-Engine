#version 430 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_uv;
layout (location = 3) in vec3 vertex_normals;

out vec4 vertexColor; 
out vec2 vertexUv;
out vec3 vertexNorms;
out vec3 fragPos;

// Fog items 
out float visibility; 
const float fogDensity = 0.0035f;
const float gradient = 6.0f;

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

	// Fog calculation (calculate distance of this vertex to camera)
	vec4 worldPos = model * vec4(vertex_position, 1.0f);
	vec4 posRelativeToCamera = view * worldPos;
	float distanceFromCamera = length(posRelativeToCamera.xyz);
	visibility = exp(-pow((distanceFromCamera * fogDensity), gradient));
}