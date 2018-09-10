#version 440 core

out vec4 FragColor;
in vec3 skyboxTexCoord;

uniform samplerCube cubemap;

uniform float brightness;
uniform float darkness;

void main()
{
    FragColor = texture(cubemap, skyboxTexCoord);
	FragColor.rgb = (FragColor.rgb - (0.5f + darkness)) * (1.0f + 0.3f) + 0.5f - brightness;
} 