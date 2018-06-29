#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Dependencies/glew/include/GL/glew.h"
#include "Physics.h"
#include "Weapon.h"

class Player
{
public:
	Player();
	~Player();

	void Init(Camera& cam);
	void ProcessInput(Camera& cam, float dt, std::vector<SDL_Event> events);
	void Update(Camera& cam, Terrain& terrain, float dt, std::vector<SDL_Event> events);
	inline int GetHealth() { return m_health; }
	bool IsPlayerWalking();
	inline bool IsPlayerJumping() { return m_jumping; }
	inline bool IsPlayerFiring() { return m_muzzleFlash; }
	inline Animation& GetAnimationComponent() { return m_animationComponent; }
	inline Weapon& GetCurrWeapon() { return m_currWeapon; }
	void Switch();

private:
	enum { CAM_FORWARD, CAM_BACKWARD, CAM_LEFT, CAM_RIGHT, CAM_RISE, CAM_FALL, CAM_JUMP, TOTAL_CAM_MOVES };
	bool m_bCamMovements[TOTAL_CAM_MOVES];

	bool m_walking, m_sprinting, m_jumping, m_isInAir, m_firing, m_reloading, m_muzzleFlash, m_usingAR;
	int m_health;
	float m_jumpHeight, m_upwardSpeed;

	Animation m_animationComponent;
	Physics m_physicsComponent;
	Weapon m_currWeapon, m_assaultRifle, m_sniperRifle;
};

#endif // !__PLAYER_H__