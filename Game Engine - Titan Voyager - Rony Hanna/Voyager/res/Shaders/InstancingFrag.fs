#version 440 core
out vec4 FragColor;

in VS_OUT 
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D meshTexture;
uniform vec3 lightPos; 
uniform vec3 viewPos;

void main()
{
	vec4 texColor = texture(meshTexture, fs_in.TexCoords);

	float ambientFactor = 0.3f;
	vec3 lightColor = texColor.rgb;
	vec3 ambient = ambientFactor * lightColor;
	
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;	

	float specularFactor = 0.07f;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
	vec3 specular = specularFactor * spec * lightColor;	
	
	vec3 result = (ambient + diffuse + specular) * vec3(texColor);
		
	FragColor = vec4(result, 1.0f);
} 