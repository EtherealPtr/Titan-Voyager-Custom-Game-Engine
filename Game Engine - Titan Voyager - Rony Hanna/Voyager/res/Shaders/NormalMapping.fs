#version 440 core
out vec4 FragColor;

in VS_OUT
{
	vec4 vertexColor;
	vec2 vertexUv;
	vec3 fragPos;
	mat3 TBN;
} fs_in;

uniform sampler2D meshTexture;
uniform sampler2D normalMap;

uniform vec3 lightPos; 
uniform vec3 viewPos;

void main()
{
	vec4 texColor = texture(meshTexture, fs_in.vertexUv); 
	if (texColor.a < 0.1f) 
		discard;
		
	vec3 fragNormal = texture(normalMap, fs_in.vertexUv).rgb;
	fragNormal = normalize(fragNormal * 2.0f - 1.0f);
	fragNormal = normalize(fs_in.TBN * fragNormal);
	
	float ambientFactor = 0.3f;
	vec3 lightColor = vec3(0.5f, 0.972f, 0.905f);
	vec3 ambient = ambientFactor * lightColor;

	vec3 lightDir = normalize(lightPos - fs_in.fragPos);

	float diff = max(dot(fragNormal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	float specularFactor = 0.5f;
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularFactor * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * vec3(texColor);

    FragColor = vec4(result, 1.0f);
} 