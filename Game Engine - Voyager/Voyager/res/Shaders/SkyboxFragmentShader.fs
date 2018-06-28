#version 430 core

out vec4 FragColor;
in vec3 skyboxTexCoord;

uniform samplerCube cubemap;

void main()
{
    FragColor = texture(cubemap, skyboxTexCoord);
} 