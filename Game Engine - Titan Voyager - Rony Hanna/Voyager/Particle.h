#pragma once
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Dependencies/glm-0.9.9-a2/glm/glm.hpp"
#include "Dependencies/glm-0.9.9-a2/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/SDL2/include/SDL.h"

class Particle
{
public:
	Particle();
	~Particle();

	bool Update(float deltaTime);
	
	void SetOrigin(glm::vec3& orig);

	glm::vec3 GetPos() { return m_position; }

private:
	glm::vec3 m_position;
	glm::vec3 m_origin;
	glm::vec3 m_velocity;
	GLfloat m_life;
};

#endif // !__PARTICLE_H__