#version 440 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 vertexUv;
in vec3 vertexNorms;
in vec3 fragPos;

uniform sampler2D meshTexture;

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform bool damaged;

struct Spotlight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	 
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

uniform Spotlight spotlight;

// Function prototype
vec3 CalculateSpotlight(Spotlight light, vec3 normal, vec3 viewDir);

void main()
{
	vec4 texColor = texture(meshTexture, vertexUv);

	float ambientFactor = 0.3f;
	vec3 lightColor = vec3(0.5f, 0.972f, 0.905f);
	vec3 ambient = ambientFactor * lightColor;

	vec3 norm = normalize(vertexNorms);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;	 

	float specularFactor = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularFactor * spec * lightColor;
	vec3 spotlightResult = CalculateSpotlight(spotlight, norm, viewDir);
	
	if (texColor.a < 0.1f) 
		discard;
	
	vec3 result;
	
	if (!damaged)
		result = (ambient + diffuse + specular + spotlightResult) * vec3(texColor);
	else
		result = vec3(0.7f, 0.0f, 0.0f);
		
    FragColor = vec4(result, 1.0f);
} 

vec3 CalculateSpotlight(Spotlight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 reflectDirection = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDirection), 0), 32);
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// Spotlight brightness
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	vec3 diffuse = light.diffuse;
	vec3 specular = light.specular;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	return (diffuse + specular);
}