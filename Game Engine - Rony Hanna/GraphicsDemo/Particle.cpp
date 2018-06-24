#include "Particle.h"
#include <iostream>
Particle::Particle() :
	m_position(0.0f, 0.0f, 0.0f),
	m_life(RandomNumBetweenTwo(0.5f, 1.5f))
{
	m_velocity = glm::vec3(RandomNumBetweenTwo(-1.0f, 1.0f), RandomNumBetweenTwo(1.0f, 5.0f), RandomNumBetweenTwo(-2.0f, 2.0f));
	m_origin = glm::vec3(0.0f, 0.0f, 0.0f);
}

Particle::~Particle()
{}

// -------------------
// Author: Rony Hanna
// Description: Function that generates a random number between 2 float values
// -------------------
float Particle::RandomNumBetweenTwo(float _min, float _max)
{
	float result = (float)rand() / (double)RAND_MAX;
	return _min + result * (_max - _min);
}

bool Particle::Update(float deltaTime)
{
	m_life -= deltaTime;

	if (m_life > 0.0f)
	{
		m_position += m_velocity * deltaTime;
		//m_color.w -= 0.01f;
	}
	else
	{
		m_position = m_origin;
		m_velocity = glm::vec3(RandomNumBetweenTwo(-1.0f, 1.0f), RandomNumBetweenTwo(1.0f, 5.0f), RandomNumBetweenTwo(-2.0f, 2.0f));
		m_life = RandomNumBetweenTwo(0.5f, 1.5f);
		//return false;
	}

	return true;
}

void Particle::SetOrigin(glm::vec3& orig)
{
	m_origin = orig;
}