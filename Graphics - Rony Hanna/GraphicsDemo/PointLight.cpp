#include "PointLight.h"

PointLight::PointLight()
{}

PointLight::~PointLight()
{}

void PointLight::Configure(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}
