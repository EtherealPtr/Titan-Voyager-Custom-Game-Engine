#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/SDL2/include/SDL.h"
#include "Animation.h"
#include "Physics.h"

class Player
{
public:
	Player();
	~Player();

	void ProcessInput(Camera& cam, float dt, std::vector<SDL_Event> events);
	void Update(Model& weapon, Camera& cam, Terrain& terrain, float dt, std::vector<SDL_Event> events);
	int GetAmmoCount() { return m_ammoCount; }
	int GetHealth() {	return m_health; }
	bool IsPlayerWalking();
	bool IsPlayerJumping() { return m_jumping; }
	bool IsPlayerFiring() { return m_muzzleFlash; }
	
private:
	enum { CAM_FORWARD, CAM_BACKWARD, CAM_LEFT, CAM_RIGHT, CAM_RISE, CAM_FALL, CAM_JUMP, TOTAL_CAM_MOVES };
	bool m_bCamMovements[TOTAL_CAM_MOVES];

	bool m_walking, m_sprinting, m_jumping, m_isInAir, m_firing, m_reloading, m_muzzleFlash;
	int m_ammoCount, m_health;
	float m_fireTimer, m_fireRate, m_reloadTimer, m_jumpHeight, m_upwardSpeed;

	Animation m_animationComponent;
	Physics m_physicsComponent;

	// Private functions
	void Fire(Model& weapon, Camera& cam, float dt);
	void Reload(Model& weapon, Camera& cam, float dt);
};

#endif // !__PLAYER_H__