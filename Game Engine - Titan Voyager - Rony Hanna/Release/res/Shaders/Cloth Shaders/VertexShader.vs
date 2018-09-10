#version 440 core

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec2 vUV;
out vec3 vNormal;

uniform mat4 mvp;
uniform mat4 view;

void main ()
{
	mat3 normalMatrix = inverse(transpose(mat3(view)));
    vNormal = normalize(normalMatrix * normal);
    vUV = uv;
    gl_Position = mvp * vec4(position, 1.0f);
}