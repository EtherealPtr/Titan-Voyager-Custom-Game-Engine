#version 430 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 vertexUv;
in vec3 fragPos;
in mat3 TBN;

uniform sampler2D meshTexture;
uniform sampler2D normalMap;

uniform vec3 lightPos; 
uniform vec3 viewPos;

void main()
{
	vec4 texColor = texture(meshTexture, vertexUv); 
	if (texColor.a < 0.1f) 
		discard;
		
	vec3 fragNormal = texture(normalMap, vertexUv).rgb;
	fragNormal = normalize(fragNormal * 2.0f - 1.0f);
	fragNormal = normalize(TBN * fragNormal);
	
	float ambientFactor = 0.3f;
	vec3 lightColor = vec3(0.5f, 0.972f, 0.905f);
	vec3 ambient = ambientFactor * lightColor;

	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(fragNormal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	float specularFactor = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularFactor * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * vec3(texColor);

    FragColor = vec4(result, 1.0f);
} 