#version 430 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 vertexNormal;
in vec3 fragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 lightPos; 
uniform vec3 viewPos;

void main()
{   
	vec4 textureColour = texture(texture_diffuse1, TexCoords);
	
	float ambientFactor = 0.3f;
	vec3 lightColor = vec3(0.97f, 0.88f, 0.70f);
	vec3 ambient = ambientFactor * lightColor;
	
	vec3 norm = normalize(vertexNormal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;	 
	
	float specularFactor = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularFactor * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * vec3(textureColour);
	
    FragColor = vec4(result, 1.0f);
}