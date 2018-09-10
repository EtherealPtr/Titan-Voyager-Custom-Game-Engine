#version 440 core
out vec4 fragColor;

uniform sampler2D mainTexture;

in vec2 vUV;
in vec3 vNormal;
 
void main()
{
	fragColor = texture(mainTexture, vUV);
}