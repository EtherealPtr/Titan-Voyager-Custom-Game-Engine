#version 440 core

struct DirectionalLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 lightColour;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic; 
	
	vec3 lightColour;
};

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

out vec4 FragColor;
in vec4 vertexColor;
in vec2 vertexUv;
in vec3 vertexNorms;
in vec3 fragPos;
in float visibility; 
in mat3 TBN;
uniform bool fogActive;
uniform float fogIntensity;

// Terrain textures
uniform sampler2D meshTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform sampler2D grassNormalMap;

uniform vec3 lightPos; 
uniform vec3 cameraDir;
uniform vec3 viewPos;

uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform Spotlight spotlight;

vec4 totalColour;

// Function prototypes
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalculateSpotlight(Spotlight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(viewPos - fragPos);
	
	// Normal mapping
	vec2 tiledCoords = vertexUv * 40.0f;
	//vec3 fragNormal = vertexNorms;
    vec3 fragNormal = texture(grassNormalMap, tiledCoords).rgb;
	fragNormal = normalize(fragNormal * 2.0f - 1.0f);
	fragNormal = normalize(TBN * fragNormal);
	
	// Multi-texturing
	vec4 blendMapColour = texture(blendMap, vertexUv);
	float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	vec4 backgroundTextureColour = texture(meshTexture, tiledCoords) * backTextureAmount; 
	vec4 rTextureColour = texture(rTexture, tiledCoords) * blendMapColour.r;
	vec4 gTextureColour = texture(gTexture, tiledCoords) * blendMapColour.g;
	vec4 bTextureColour = texture(bTexture, tiledCoords) * blendMapColour.b;
	totalColour = backgroundTextureColour + rTextureColour + gTextureColour + bTextureColour;
	
	vec3 directionLightResult = CalculateDirectionalLight(dirLight, fragNormal, viewDir);
	vec3 pointLightResult = CalculatePointLight(pointLight, fragNormal, viewDir);
	vec3 spotlightResult = CalculateSpotlight(spotlight, fragNormal, viewDir);
	
	FragColor = vec4(directionLightResult + pointLightResult + spotlightResult, 1.0f) * totalColour;
	
	// Apply fog effect if the fog is active
	if (fogActive)
		FragColor = mix(vec4(0.0f, 0.0f, 0.0f, 1.0f), FragColor, visibility);
} 

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0f);
	
	// Specular
	vec3 reflectDirection = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0f), 32);
	
	// Add results
	vec3 ambient = light.ambient * dirLight.lightColour;
	vec3 diffuse = light.diffuse * dirLight.lightColour;
	vec3 specular = light.specular * dirLight.lightColour;
	
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 reflectDirection = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDirection), 0), 32);
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// Check if user specified a colour, if not then set light colour to default white colour
	if (light.lightColour.x <= 0.0f && light.lightColour.y <= 0.0f && light.lightColour.z <= 0.0f)
	{
		light.lightColour = vec3(1.0f, 1.0f, 1.0f);
	}
	
	vec3 ambient = light.ambient * totalColour.rgb * light.lightColour;
	vec3 diffuse = light.diffuse * totalColour.rgb * light.lightColour;
	vec3 specular = light.specular * totalColour.rgb * light.lightColour;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
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
	
	vec3 diffuse = light.diffuse * totalColour.rgb;
	vec3 specular = light.specular * totalColour.rgb;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	return (diffuse + specular);
}