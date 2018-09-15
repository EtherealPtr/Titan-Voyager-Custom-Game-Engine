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

	void Draw(short int id, short int enemyProjectileId);
	void DrawShockwave(short int enemyDroneBlastId);
	void Update(Terrain& terrain, Camera& cam, float dt);
	void ReduceHealth(int amount);
	
	void SetPos(glm::vec3& pos) { m_pos = pos; }
	glm::vec3& GetPos() { return m_pos; }

	float CalcDistance(glm::vec3& enemyPos, glm::vec3& playerPos);
	float CalcDistanceNoHeight(glm::vec3& enemyPos, glm::vec3& playerPos);

	void SetAttackDamage(float attkDmg) { m_attackDamage = attkDmg; }
	float GetAttackDamage() { return m_attackDamage; }

	void SetDeath(bool isDead) { m_dead = isDead; }
	bool GetDeath() { return m_dead; }

	void SetRespawnStatus(bool canRespawn);
	bool GetRespawnStatus() { return m_canRespawn; }

private:
	Camera m_camera;
	glm::vec3 m_pos, m_velocity, m_fireDir, m_dronePos, m_oldPlayerPos;
 
	float m_maximumSpeed, m_maximumDroneSpeed;
	float m_attackDamage;
	float m_deltaTime, m_currLifeTimer, m_respawnTimer;
	float m_distance, m_damageTakenDuration, m_evadeDurationCounter, m_shootDuration, m_blastRadius;
	int m_health;
	bool m_dead, m_withinAttackRange, m_takingDamage, m_evade, m_evadeRight, m_droneStatus, m_droneActive, m_fire, m_canRespawn, m_droneSelfDestruct;
	bool m_damageToken;
	ParticleEmitter m_particleEffect;

	// Private functions
	void Seek(Camera& target, const float dt);
	void Flee(Camera& target, const float dt);
	void Fire(Camera& target, Terrain& terrain, const float dt);
	void Respawn();
};

#endif // !__ENEMY_H__