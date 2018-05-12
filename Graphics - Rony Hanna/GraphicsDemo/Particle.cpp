#include "Particle.h"

Particle::Particle() :
	m_position(0.0f, 0.0f, 0.0f),
	m_color(glm::vec4(10.0f, 1.0f, 0.0f, 1.0f)),
	m_life(20.0f)
{
	m_velocity.x = ((rand() % 10) + 1);
	m_velocity.y = 2.0f;
	m_velocity.z = 0.0f;
}

Particle::~Particle()
{}

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
		return false;
	}

	return true;
}