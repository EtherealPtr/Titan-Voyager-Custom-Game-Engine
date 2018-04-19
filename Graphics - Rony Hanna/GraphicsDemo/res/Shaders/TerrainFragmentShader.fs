#version 430 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 vertexUv;
in vec3 vertexNorms;
in vec3 fragPos;

uniform sampler2D meshTexture;
uniform sampler2D normalMap;

uniform vec3 lightPos; 
uniform vec3 viewPos;

void main()
{
	float ambientFactor = 0.3f;
	vec3 lightColor = vec3(0.7f, 0.7f, 0.7f);
	vec3 ambient = ambientFactor * lightColor;

	vec3 norm = normalize(vertexNorms);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
	vec4 texColor = texture(meshTexture, vertexUv); 

	float specularFactor = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularFactor * spec * lightColor;

	vec3 result = (ambient + diffuse) * vec3(texColor);

    FragColor = vec4(result, 1.0f);
} 