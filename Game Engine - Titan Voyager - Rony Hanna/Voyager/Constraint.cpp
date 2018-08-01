#include "Constraint.h"

Constraint::Constraint(ClothParticle* p1, ClothParticle* p2) :
	m_pParticleOne(p1),
	m_pParticleTwo(p2)
{
	glm::vec3 v = p1->GetPos() - p2->GetPos();
	m_restDistance = glm::length(v);
}

Constraint::~Constraint()
{}

void Constraint::SatisfyConstraint()
{
	glm::vec3 distance = m_pParticleTwo->GetPos() - m_pParticleOne->GetPos(); 
	float currDistance = glm::length(distance);
	glm::vec3 offsetVec = distance * (1.0f - m_restDistance / currDistance);
	glm::vec3 offsetVecHalf = offsetVec * 0.5f;
	m_pParticleOne->OffsetPos(offsetVecHalf);
	m_pParticleTwo->OffsetPos(-offsetVecHalf);
}