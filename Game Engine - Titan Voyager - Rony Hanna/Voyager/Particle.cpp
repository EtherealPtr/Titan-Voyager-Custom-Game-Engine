#include "Particle.h"
#include "Utils.h"

Particle::Particle() :
	m_position(0.0f, 0.0f, 0.0f),
	m_life(Utils::GetInstance().RandomNumBetweenTwo(0.5f, 1.5f))
{
	m_velocity = glm::vec3(Utils::GetInstance().RandomNumBetweenTwo(-1.0f, 1.0f), Utils::GetInstance().RandomNumBetweenTwo(1.0f, 5.0f), Utils::GetInstance().RandomNumBetweenTwo(-2.0f, 2.0f));
	m_origin = glm::vec3(0.0f, 0.0f, 0.0f);
}

Particle::~Particle()
{}

bool Particle::Update(float deltaTime)
{
	m_life -= deltaTime;

	if (m_life > 0.0f)
	{
		m_position += m_velocity * deltaTime;
	}
	else
	{
		m_position = m_origin;
		m_velocity = glm::vec3(Utils::GetInstance().RandomNumBetweenTwo(-2.0f, 2.0f), Utils::GetInstance().RandomNumBetweenTwo(5.0f, 10.0f), Utils::GetInstance().RandomNumBetweenTwo(-4.0f, 4.0f));
		m_life = Utils::GetInstance().RandomNumBetweenTwo(1.5f, 2.5f);
	}

	return true;
}

void Particle::SetOrigin(glm::vec3& orig)
{
	m_origin = orig;
}