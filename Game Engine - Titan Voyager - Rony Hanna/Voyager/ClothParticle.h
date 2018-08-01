#pragma once
#ifndef __CLOTHPARTICLE_H__
#define __CLOTHPARTICLE_H__

#include "Dependencies/SDL2/include/SDL.h"
#include "Dependencies/glm-0.9.9-a2/glm/glm.hpp"
#include "Dependencies/glm-0.9.9-a2/glm/gtx/transform.hpp"

class ClothParticle
{
public:
	ClothParticle();
	ClothParticle(glm::vec3 pos);
	~ClothParticle();

	void VerletIntegration();
	void AddForce(glm::vec3 force)			{ m_acceleration += force / m_mass; }
	glm::vec3& GetPos()						{ return m_pos; }
	glm::vec3& GetNormal()					{ return m_accumulatedNormal; }
	void ZeroAcceleration()					{ m_acceleration = glm::vec3(0.0f); }
	void ZeroNormal()						{ m_accumulatedNormal = glm::vec3(0.0f); }
	void OffsetPos(const glm::vec3& v)		{ if (m_movable) m_pos += v; }
	void AddToNormal(glm::vec3& normal)		{ m_accumulatedNormal += glm::normalize(normal); }
	void Pin()								{ m_movable = false; }

private:
	float m_damping, m_timeStep;
	bool m_movable;
	float m_mass;
	glm::vec3 m_pos, m_oldPos, m_acceleration, m_accumulatedNormal;
};

#endif // !__CLOTHPARTICLE_H__