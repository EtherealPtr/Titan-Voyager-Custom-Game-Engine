#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Configure(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}