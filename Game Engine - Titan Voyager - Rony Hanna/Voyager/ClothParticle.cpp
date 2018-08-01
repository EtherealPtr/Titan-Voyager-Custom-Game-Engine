#include "ClothParticle.h"

ClothParticle::ClothParticle()
{}

ClothParticle::ClothParticle(glm::vec3 pos) :
	m_pos(pos),
	m_oldPos(pos),
	m_mass(1.0f),
	m_acceleration(glm::vec3(0.0f)),
	m_movable(true),
	m_accumulatedNormal(glm::vec3(0.0f)),
	m_damping(0.1f),
	m_timeStep(0.25f)
{}

ClothParticle::~ClothParticle()
{}

void ClothParticle::VerletIntegration()
{
	if (m_movable)
	{
		glm::vec3 temp = m_pos;
		m_pos = m_pos + (m_pos - m_oldPos) * (1.0f - m_damping) + m_acceleration * m_timeStep;
		m_oldPos = temp;
		m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}