#pragma once
#ifndef __CONSTRAINT_H__
#define __CONSTRAINT_H__

#include "ClothParticle.h"

class Constraint
{
public:
	Constraint(ClothParticle* p1, ClothParticle* p2);
	~Constraint();

	void SatisfyConstraint();

	ClothParticle* GetParticleOne() { return m_pParticleOne; }
	ClothParticle* GetParticleTwo() { return m_pParticleTwo; }

private:
	ClothParticle* m_pParticleOne, *m_pParticleTwo;
	float m_restDistance;
};

#endif // !__CONSTRAINT_H__