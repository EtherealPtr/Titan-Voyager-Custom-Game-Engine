#version 430 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 vertexUv;
in vec3 vertexNorms;
in vec3 fragPos;
in float visibility; 
in mat3 TBN;
vec3 fogColour = vec3(0.97f, 0.88f, 0.70f);

// Terrain textures
uniform sampler2D meshTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform sampler2D grassNormalMap;

uniform vec3 lightPos; 
uniform vec3 viewPos;

void main()
{
	vec2 tiledCoords = vertexUv * 40.0f;
	
	// Normal mapping
	vec3 fragNormal = vertexNorms;
    fragNormal = texture(grassNormalMap, tiledCoords).rgb;
	fragNormal = normalize(fragNormal * 2.0f - 1.0f);
	fragNormal = normalize(fragNormal * TBN);
	
	vec4 blendMapColour = texture(blendMap, vertexUv);
	float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	vec4 backgroundTextureColour = texture(meshTexture, tiledCoords) * backTextureAmount; 
	vec4 rTextureColour = texture(rTexture, tiledCoords) * blendMapColour.r;
	vec4 gTextureColour = texture(gTexture, tiledCoords) * blendMapColour.g;
	vec4 bTextureColour = texture(bTexture, tiledCoords) * blendMapColour.b;
	vec4 totalColour = backgroundTextureColour + rTextureColour + gTextureColour + bTextureColour;

	float ambientFactor = 0.3f;
	vec3 lightColor = vec3(0.7f, 0.7f, 0.4f);
	vec3 ambient = ambientFactor * lightColor;

	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
	vec4 texColor = texture(meshTexture, vertexUv); 

	float specularFactor = 1.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularFactor * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * vec3(totalColour);

    FragColor = vec4(result, 1.0f);
	FragColor = mix(vec4(fogColour, 1.0f), FragColor, visibility);
} 