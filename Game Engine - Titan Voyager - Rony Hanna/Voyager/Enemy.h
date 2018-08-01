#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Model.h"
#include "Camera.h"
#include "Terrain.h"
#include "ParticleEmitter.h"

class Enemy
{
public:
	Enemy(Camera& cam);
	~Enemy();

	void Draw(short int id);
	void Update(Terrain& terrain, Camera& cam, float dt);
	void ReduceHealth(int amount);
	
	void SetPos(glm::vec3& pos) { m_pos = pos; }
	glm::vec3& GetPos() { return m_pos; }

	float CalcDistance(glm::vec3& enemyPos, glm::vec3& playerPos);

	void SetAttackDistance(float attkDistance) { m_attackDistance = attkDistance; }
	float GetAttackDistance() { return m_attackDistance; }

private:
	Camera m_camera;
	Model m_model;
	glm::vec3 m_pos, m_velocity;
	float m_maximumSpeed;
	float m_deltaTime;
	float m_distance, m_attackDistance, m_damageTakenDuration, m_evadeDurationCounter;
	int m_health;
	bool m_dead, m_withinAttackRange, m_takingDamage, m_evade, m_evadeRight;
	ParticleEmitter m_particleEffect;

	// Private functions
	void Seek(Camera& target, float dt);
	void Flee(Camera& target, float dt);
};

#endif // !__ENEMY_H__