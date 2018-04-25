#pragma once
#ifndef __DIRECTIONALLIGHT_H__
#define __DIRECTIONALLIGHT_H__

#include "Dependencies\glm-0.9.9-a2\glm\glm.hpp"
#include "Dependencies\glm-0.9.9-a2\glm\gtx\transform.hpp"

class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

	void Configure(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	void SetDirection(glm::vec3 dir) { m_direction = dir; }
	glm::vec3& GetDirection() { return m_direction; }

	glm::vec3& GetAmbient() { return m_ambient; }
	glm::vec3& GetDiffuse() { return m_diffuse; }
	glm::vec3& GetSpecular() { return m_specular; }

private:
	glm::vec3 m_direction;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

#endif // !__DIRECTIONALLIGHT_H__