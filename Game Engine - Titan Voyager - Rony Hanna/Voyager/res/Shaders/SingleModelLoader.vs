#version 440 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_Uv;

out vec2 TexCoords;
out vec3 vertexNormal;
out vec3 fragPos;

// Fog items 
out float visibility; 
const float fogDensity = 0.0022f;
const float gradient = 7.0f;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = vertex_Uv;    
    gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
	vertexNormal = vertex_normal;
	fragPos = vec3(model * vec4(vertex_position, 1.0f));
	
	// Fog calculation (calculate distance of this vertex to camera)
	vec4 worldPos = model * vec4(vertex_position, 1.0f);
	vec4 posRelativeToCamera = view * worldPos;
	float distanceFromCamera = length(posRelativeToCamera.xyz);
	visibility = exp(-pow((distanceFromCamera * fogDensity), gradient));
}