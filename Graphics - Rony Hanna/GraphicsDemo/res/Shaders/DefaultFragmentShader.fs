#version 430 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 vertexUv;
in vec3 vertexNorms;
in vec3 fragPos;

uniform sampler2D meshTexture;
uniform vec3 lightPos; 

void main()
{
	float ambientFactor = 0.3f;
	vec4 ambientLightColor = vec4(0.5f, 0.972f, 0.905f, 1.0f);
	vec4 ambientLighting = ambientFactor * ambientLightColor;

	vec3 norm = normalize(vertexNorms);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec4 diffuse = diff * ambientLightColor;
	vec4 texColor = texture(meshTexture, vertexUv); 

	if (texColor.a < 0.1f) 
		discard;
	
	vec4 result = (ambientLighting + diffuse) * texColor;

    FragColor = result;
} 