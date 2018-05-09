#include "Particle.h"

Particle::Particle() :
	m_position(0.0f),
	m_velocity(0.0f),
	m_color(1.0f),
	m_life(1.0f)
{}

Particle::~Particle()
{
}

void Particle::Update(float deltaTime)
{
	m_life -= deltaTime;

	if (m_life > 0.0f)
	{
		m_position += m_velocity * deltaTime;
		m_color.a -= deltaTime * 2.5f;
	}
}